##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=dataServer
ConfigurationName      :=Debug
WorkspacePath          := "/home/parisbre56/.codelite/DataTransfer"
ProjectPath            := "/home/parisbre56/.codelite/DataTransfer"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=parisbre56
Date                   :=07/03/14
CodeLitePath           :="/home/parisbre56/.codelite"
LinkerName             :=/usr/bin/g++ 
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="dataServer.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -pthread
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++ 
CC       := /usr/bin/gcc 
CXXFLAGS :=  -g -O0 -Wall -std=c++0x -pthread $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/DS_Main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Pool.cpp$(ObjectSuffix) $(IntermediateDirectory)/ConnectionData.cpp$(ObjectSuffix) $(IntermediateDirectory)/DataObject.cpp$(ObjectSuffix) $(IntermediateDirectory)/connectionReaderThread.cpp$(ObjectSuffix) $(IntermediateDirectory)/ThreadList.cpp$(ObjectSuffix) $(IntermediateDirectory)/workerThread.cpp$(ObjectSuffix) $(IntermediateDirectory)/writeLock.cpp$(ObjectSuffix) $(IntermediateDirectory)/DataObjectList.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/DS_Main.cpp$(ObjectSuffix): DS_Main.cpp $(IntermediateDirectory)/DS_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/DS_Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DS_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DS_Main.cpp$(DependSuffix): DS_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DS_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DS_Main.cpp$(DependSuffix) -MM "DS_Main.cpp"

$(IntermediateDirectory)/DS_Main.cpp$(PreprocessSuffix): DS_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DS_Main.cpp$(PreprocessSuffix) "DS_Main.cpp"

$(IntermediateDirectory)/Pool.cpp$(ObjectSuffix): Pool.cpp $(IntermediateDirectory)/Pool.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/Pool.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Pool.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Pool.cpp$(DependSuffix): Pool.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Pool.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Pool.cpp$(DependSuffix) -MM "Pool.cpp"

$(IntermediateDirectory)/Pool.cpp$(PreprocessSuffix): Pool.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Pool.cpp$(PreprocessSuffix) "Pool.cpp"

$(IntermediateDirectory)/ConnectionData.cpp$(ObjectSuffix): ConnectionData.cpp $(IntermediateDirectory)/ConnectionData.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/ConnectionData.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ConnectionData.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ConnectionData.cpp$(DependSuffix): ConnectionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ConnectionData.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ConnectionData.cpp$(DependSuffix) -MM "ConnectionData.cpp"

$(IntermediateDirectory)/ConnectionData.cpp$(PreprocessSuffix): ConnectionData.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ConnectionData.cpp$(PreprocessSuffix) "ConnectionData.cpp"

$(IntermediateDirectory)/DataObject.cpp$(ObjectSuffix): DataObject.cpp $(IntermediateDirectory)/DataObject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/DataObject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DataObject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DataObject.cpp$(DependSuffix): DataObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DataObject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DataObject.cpp$(DependSuffix) -MM "DataObject.cpp"

$(IntermediateDirectory)/DataObject.cpp$(PreprocessSuffix): DataObject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DataObject.cpp$(PreprocessSuffix) "DataObject.cpp"

$(IntermediateDirectory)/connectionReaderThread.cpp$(ObjectSuffix): connectionReaderThread.cpp $(IntermediateDirectory)/connectionReaderThread.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/connectionReaderThread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/connectionReaderThread.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/connectionReaderThread.cpp$(DependSuffix): connectionReaderThread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/connectionReaderThread.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/connectionReaderThread.cpp$(DependSuffix) -MM "connectionReaderThread.cpp"

$(IntermediateDirectory)/connectionReaderThread.cpp$(PreprocessSuffix): connectionReaderThread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/connectionReaderThread.cpp$(PreprocessSuffix) "connectionReaderThread.cpp"

$(IntermediateDirectory)/ThreadList.cpp$(ObjectSuffix): ThreadList.cpp $(IntermediateDirectory)/ThreadList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/ThreadList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ThreadList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ThreadList.cpp$(DependSuffix): ThreadList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ThreadList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ThreadList.cpp$(DependSuffix) -MM "ThreadList.cpp"

$(IntermediateDirectory)/ThreadList.cpp$(PreprocessSuffix): ThreadList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ThreadList.cpp$(PreprocessSuffix) "ThreadList.cpp"

$(IntermediateDirectory)/workerThread.cpp$(ObjectSuffix): workerThread.cpp $(IntermediateDirectory)/workerThread.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/workerThread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/workerThread.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/workerThread.cpp$(DependSuffix): workerThread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/workerThread.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/workerThread.cpp$(DependSuffix) -MM "workerThread.cpp"

$(IntermediateDirectory)/workerThread.cpp$(PreprocessSuffix): workerThread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/workerThread.cpp$(PreprocessSuffix) "workerThread.cpp"

$(IntermediateDirectory)/writeLock.cpp$(ObjectSuffix): writeLock.cpp $(IntermediateDirectory)/writeLock.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/writeLock.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/writeLock.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/writeLock.cpp$(DependSuffix): writeLock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/writeLock.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/writeLock.cpp$(DependSuffix) -MM "writeLock.cpp"

$(IntermediateDirectory)/writeLock.cpp$(PreprocessSuffix): writeLock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/writeLock.cpp$(PreprocessSuffix) "writeLock.cpp"

$(IntermediateDirectory)/DataObjectList.cpp$(ObjectSuffix): DataObjectList.cpp $(IntermediateDirectory)/DataObjectList.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/DataObjectList.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DataObjectList.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DataObjectList.cpp$(DependSuffix): DataObjectList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DataObjectList.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/DataObjectList.cpp$(DependSuffix) -MM "DataObjectList.cpp"

$(IntermediateDirectory)/DataObjectList.cpp$(PreprocessSuffix): DataObjectList.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DataObjectList.cpp$(PreprocessSuffix) "DataObjectList.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/DS_Main.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DS_Main.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DS_Main.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Pool.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Pool.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Pool.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ConnectionData.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ConnectionData.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ConnectionData.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DataObject.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DataObject.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DataObject.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/connectionReaderThread.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/connectionReaderThread.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/connectionReaderThread.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ThreadList.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ThreadList.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ThreadList.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/workerThread.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/workerThread.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/workerThread.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/writeLock.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/writeLock.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/writeLock.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DataObjectList.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DataObjectList.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DataObjectList.cpp$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/dataServer"


