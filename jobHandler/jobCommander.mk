##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=jobCommander
ConfigurationName      :=Debug
WorkspacePath          := "/home/parisbre56/.codelite/jobHandler"
ProjectPath            := "/home/parisbre56/.codelite/jobHandler"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=parisbre56
Date                   :=06/15/14
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
ObjectsFileList        :="jobCommander.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lrt -pthread
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
CXXFLAGS :=  -g -O0 -Wall -std=c++0x $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/CM_Main.cpp$(ObjectSuffix) $(IntermediateDirectory)/shared_data.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/CM_Main.cpp$(ObjectSuffix): CM_Main.cpp $(IntermediateDirectory)/CM_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/jobHandler/CM_Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CM_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CM_Main.cpp$(DependSuffix): CM_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CM_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CM_Main.cpp$(DependSuffix) -MM "CM_Main.cpp"

$(IntermediateDirectory)/CM_Main.cpp$(PreprocessSuffix): CM_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CM_Main.cpp$(PreprocessSuffix) "CM_Main.cpp"

$(IntermediateDirectory)/shared_data.cpp$(ObjectSuffix): shared_data.cpp $(IntermediateDirectory)/shared_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/jobHandler/shared_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/shared_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/shared_data.cpp$(DependSuffix): shared_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/shared_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/shared_data.cpp$(DependSuffix) -MM "shared_data.cpp"

$(IntermediateDirectory)/shared_data.cpp$(PreprocessSuffix): shared_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/shared_data.cpp$(PreprocessSuffix) "shared_data.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/CM_Main.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/CM_Main.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/CM_Main.cpp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/shared_data.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/shared_data.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/shared_data.cpp$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/jobCommander"


