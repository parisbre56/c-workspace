##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=remoteClient
ConfigurationName      :=Debug
WorkspacePath          := "/home/parisbre56/.codelite/DataTransfer"
ProjectPath            := "/home/parisbre56/.codelite/DataTransfer"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=parisbre56
Date                   :=06/30/14
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
ObjectsFileList        :="remoteClient.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
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
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/RC_Main.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/RC_Main.cpp$(ObjectSuffix): RC_Main.cpp $(IntermediateDirectory)/RC_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/parisbre56/.codelite/DataTransfer/RC_Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/RC_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/RC_Main.cpp$(DependSuffix): RC_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/RC_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/RC_Main.cpp$(DependSuffix) -MM "RC_Main.cpp"

$(IntermediateDirectory)/RC_Main.cpp$(PreprocessSuffix): RC_Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/RC_Main.cpp$(PreprocessSuffix) "RC_Main.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/RC_Main.cpp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/RC_Main.cpp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/RC_Main.cpp$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/remoteClient"


