#
# Copyright Regents of the University of Minnesota.  This software is released under the following license: [http://opensource.org/licenses/lgpl-3.0.html].  
# Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
#
# Funding for this software development was provided in part by the NSF (IIS-1251069) and NIH (1R01EB018205-01).
# Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
#
# The content is solely the responsibility of the authors and does not necessarily represent the official views of the National Institutes of Health.
#
# Code author(s):
#	Chi-Lun (Cyrus) Lin
#

from odbAccess import *
from sys import argv,exit
import __main__
from array import array
from decimal import Decimal
import os
from operator import itemgetter, attrgetter
import netCDF4
from netCDF4 import Dataset

def ReadData(odbPath, outFile):
	##
	# FOR DEBUG
	bReadMesh = 1
	bReadResult = 1
	
	print 'Reading ODB:' + odbPath
	print 'Output to: ' + outFile

	# Initialization     
	print 'Open ', odbPath
	odb = openOdb(odbPath) # Open ABAQUS .odb file
	asm = odb.rootAssembly
	instances = asm.instances
	root = Dataset(outFile, 'w', format='NETCDF4') # Create a netCDF file
	root.description = 'ABAQUS Output Database, version: ' + odb.jobData.version
	root.source = odb.name
	root.history = 'Created ' + odb.jobData.creationTime

	###
	# Get model data: assembly, parts, sections, materials
	###
		
	if bReadMesh == 1:
		print 'Begin processing model data...'
		grpMesh = root.createGroup('mesh')

		# Create instances in the assembly
		print 'Number of instances = ', len(instances)
		for inst in instances.keys():
			##
			# Create a group for this instance
			grpInst = grpMesh.createGroup(inst)
			print 'Instance Name : ', inst
			# nodes stored in nodes variable
			nodeNo = len(instances[inst].nodes)
			grpInst.createDimension('nodeNo', nodeNo)
			grpInst.createDimension('coordinate', 3) # (x,y,z)
			nodes = grpInst.createVariable('nodes', 'f4', ('nodeNo', 'coordinate'))
			print 'Node Number = ', nodeNo
			for n in instances[inst].nodes:
				index = n.label
				nodes[index-1] = n.coordinates

			## Elements stored in elements variable
			elementNo = len(instances[inst].elements)
			grpInst.createDimension('elementNo', elementNo)
			elementSize = len(instances[inst].elements[1].connectivity)
			grpInst.createDimension('connectivity', elementSize)
			elements = grpInst.createVariable('elements', 'i4', ('elementNo', 'connectivity'))
			print 'Element Number = ', elementNo
			for e in instances[inst].elements:
				index = e.label
				elements[index-1] = e.connectivity
		print 'End processing model data'

	##
	# Get results data
	print 'Begin processing results data...'
	steps = odb.steps
	grpResults = root.createGroup('results')
	
	## 
	# Iterate simulation steps
	for sKey in steps.keys():
		print 'Step:', sKey
		grpStep = grpResults.createGroup(sKey)
		frameIndex = 0
		##
		# Read field request output
		for frame in steps[sKey].frames:  
			frameName = 'frame_%d' %frameIndex
			
			# Get frame info
			print 'Frame Name : ', frameName
			
			# Create a group for this frame
			grpFrame = grpStep.createGroup(frameName)
			grpFrame.createDimension('time', 1)
			grpFrame.createDimension('ID', 1)
			frameID = grpFrame.createVariable('frameID', 'i4', ('ID'))
			frameID = frameIndex
			stepTime = grpFrame.createVariable('stepTime', 'f4', ('time'))
			stepTime[:] = frame.frameValue

			# Print frame info
			print 'Frame ID : ', frameID
			print 'Frame Step Time : ', stepTime
			
			# Get available output fields
			fieldKeys = []
			desiredFields = ['S', 'U', 'STATUS','ABC']
			print 'Available output fields : ', frame.fieldOutputs.keys()
			for key in frame.fieldOutputs.keys():
				if any(key == str for str in desiredFields):		
					fieldKeys.append(key)
			print 'Output fields to be processed : ', fieldKeys		 
			
			for field in fieldKeys:
				#grpField = grpFrame.createGroup(field)
				#grpField.createDimension('InstNo', len(instances))
				#grpField.createDimension('Node', len(instances))
				print 'Process output field : ', field
				
				# Create groups for instances
				instIndex = 0
				for inst in instances.keys():
					# Get instance info
					print 'Process instance : ', inst
					nodeNo = len(instances[inst].nodes)
					print 'Node number = ', nodeNo
				
					# Check if the group for the instance is already created. Avoid the error for recreating
					if inst in grpFrame.groups:
						grpInst = grpFrame.groups[inst]
						grpElm = grpFrame.groups[inst].groups['elementData']
						grpNode = grpFrame.groups[inst].groups['nodalData']
					else:
						grpInst = grpFrame.createGroup(inst)
						grpElm = grpInst.createGroup('elementData')
						grpNode = grpInst.createGroup('nodalData')
						grpInst.createDimension('elementNo', elementNo)
						grpInst.createDimension('nodeNo', nodeNo)
					bNeedAvg = 1;
					if field == 'U':
						set = frame.fieldOutputs[field].getSubset(position=NODAL,region=odb.rootAssembly.instances[inst])
						bNeedAvg = 0;
						tensorName = 'Tensor_U'
						dataType = 'NODE'
					elif field == 'S':
						set = frame.fieldOutputs[field].getSubset(position=ELEMENT_NODAL,region=odb.rootAssembly.instances[inst])
						bNeedAvg = 1;
						tensorName = 'Tensor_S'
						dataType = 'NODE'
					elif field == 'STATUS':
						set = frame.fieldOutputs[field].getSubset(position=WHOLE_ELEMENT,region=odb.rootAssembly.instances[inst])
						bNeedAvg = 0;
						tensorName = ''
						dataType = 'ELEMENT'
					else:
						set = frame.fieldOutputs[field].getSubset(position=ELEMENT_NODAL,region=odb.rootAssembly.instances[inst])
						bNeedAvg = 1;
						tensorName = 'Tensor_' + field
						dataType = 'NODE'
				
					# Check if contains data
					if len(set.values) > 0:
						if dataType == 'NODE':
							# Process tensors
							grpInst.createDimension(tensorName, len(set.componentLabels))
							nodalData = grpNode.createVariable(field, 'f4', ('nodeNo', tensorName))
							tensorData = [] # a list of the tensor of the field. Ex: [(U1, U2, U3)...]
							subsets = []
							componentNum = 0
							varDesc = ''
							for label in set.componentLabels:  # Get tensor components
								print 'Process tensor component : ', label
								varDesc = varDesc + ', ' + label
								subset = set.getScalarField(componentLabel=label)
								# Get data from subset
								data = []
								processedData = []
								# sort data, average if needed
								if bNeedAvg == 0:
									for value in subset.values:
										data.append((value.nodeLabel,value.data))
									sortedData = sorted(data, key=itemgetter(0))
									# Eliminate the nodeIndex column
									for value in sortedData:
										processedData.append(value[1]) 
								else:
									for value in subset.values:
										data.append((value.nodeLabel,value.elementLabel,value.data))
									processedData = AvgNodalElementData(data)
								print 'size= ', len(processedData)
								componentNum = componentNum+1
								tensorData.append(processedData)
						
							# Add tensor data into the netCDF4 variable, nodalData	
							tmp = []
							for j in range(0,len(tensorData[0])):
								for k in range(0,componentNum):
									tmp.append(tensorData[k][j])
								nodalData[j] = tmp
								tmp = []							

							nodalData.description = varDesc
						
							# Process invariants
							for var in set.validInvariants:    # Get invariants
								print 'Process invariant : ', var
								subset = set.getScalarField(invariant=var)
								data = []
								for value in subset.values:
									data.append((value.nodeLabel,value.elementLabel,value.data))
								nodalData = grpNode.createVariable(var.name, 'f4', ('nodeNo'))
								tmp = AvgNodalElementData(data)
								print 'size= ', len(tmp)
								nodalData[:] = AvgNodalElementData(data)
						elif dataType == 'ELEMENT':
							#if field == 'STATUS':
							data = []
							for value in set.values:
								data.append((value.elementLabel,value.data))
							elementData = grpElm.createVariable(field, 'f4', ('elementNo'))
							sortedData = sorted(data, key=itemgetter(0))
							# Eliminate the nodeIndex column
							tmp = []
							for value in sortedData:
								tmp.append(value[1])
							elementData[:] = tmp
			frameIndex = frameIndex + 1
		
############################################################################
## THIS BLOCK IS FOR READING ABAQUS HISTORY DATA
## SKIP FOR NOW
		# Read history request output
		# grpHistData = grpStep.createGroup('historyData')
# 		desiredHistRegions = ['Node CUTTER_ROUNDED-1.13057']
# 		desiredHistFields = ['RF3', 'RM3']
# 		histRegionKeys = []
# 		print 'Available history regions : ', steps[sKey].historyRegions.keys()
# 		for key in steps[sKey].historyRegions.keys():
# 			if any(key == str for str in desiredHistRegions):		
# 				histRegionKeys.append(key)
# 		print 'History regions to be processed : ', histRegionKeys
		
		## 
		# Begin reading regional data
# 		for regionName in histRegionKeys:
# 			print 'Process history region : ', regionName
# 			grpHistRegion = grpHistData.createGroup(regionName)
# 			histRegionData = steps[sKey].historyRegions[regionName]
# 			histFieldKeys = []
# 			print 'Available history output fields : ', histRegionData.historyOutputs.keys()
# 			for key in histRegionData.historyOutputs.keys():
# 				if any(key == str for str in desiredHistFields):		
# 					histFieldKeys.append(key)
# 			print 'History output fields to be processed : ', histFieldKeys
# 			
# 			## 
# 			# Begin reading history field output
# 			for fieldName in histFieldKeys:
# 				print 'Process history data field : ', fieldName
# 				data = readHistoryOutput(odb.steps[sKey], regionName, fieldName)
# 				dimensionName = 'timeSteps'
# 				if dimensionName not in grpHistRegion.dimensions:
# 					grpHistRegion.createDimension(dimensionName, len(data))
# 					grpHistRegion.createDimension('timeData', 2)
# 				histFieldData = grpHistRegion.createVariable(fieldName, 'f4', ('timeSteps', 'timeData'))
# 				histFieldData[:] = data
##################################################################################
			
	print 'End processing results data'
	odb.close()
	root.close()
	print 'Data has been wrriten to: ' +  outFile
##
# This function reads history field output as specified step, field and region
# and returns a timeSteps x 2 vector
def readHistoryOutput(step, region, field):
	print region
	print field
	odbField = step.historyRegions[region].historyOutputs[field]
	data = []
	for value in odbField.data:
		data.append(value)
	return data
##
# Average element nodal values and output to nodal values
def AvgNodalElementData(Data): 
    SortedData = sorted(Data, key=itemgetter(0)) # sort by node
    NodalData =  [];
    ### Do averaging
    curNode = 1
    preNode = 1
    sum = 0
    count = 0
    for i in range(1,len(SortedData)+1):
        tmp = SortedData[i-1] # (NodeLabel, ElementLabel, Value)
        curNode = tmp[0]
        if i == 1:               # sync preNode and curNode at first line
            preNode = curNode
        if curNode <> preNode:   # node index changed
            if count > 0:
                avg = sum/count
                NodalData.append(avg)
                sum = 0
                count = 0
        if i == len(SortedData):  # Last line
            if curNode <> preNode: # the last node contains only one line
                # no need to do averaging
                NodalData.append(tmp[2])
            if curNode == preNode: # the last node contains multiple lines
                # add this last value and do averaging
                sum = sum + tmp[2]
                count = count + 1
                avg = sum/count
                NodalData.append(avg)
        # count and sum
        sum = sum + tmp[2]
        count = count + 1
        preNode = curNode
    return NodalData
    
if __name__ == '__main__':
	argList = argv
	print argList[0]
	print argList[1]
	print argList[2]
	odb = argList[1]
	outFile = argList[2]
	ReadData(odb, outFile)
