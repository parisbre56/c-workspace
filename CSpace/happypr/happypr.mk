##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=happypr
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/happypr"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=11/29/2011
CodeLitePath           :="/home/paris/.codelite"
LinkerName             :=gcc
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=gcc
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="/home/paris/.codelite/CSpace/happypr/happypr.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
CmpOptions             := -g $(Preprocessors)
C_CmpOptions           := -g $(Preprocessors)
LinkOptions            :=  -lm
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/happypr$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/happypr$(ObjectSuffix): happypr.c $(IntermediateDirectory)/happypr$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/happypr/happypr.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/happypr$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/happypr$(DependSuffix): happypr.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/happypr$(ObjectSuffix) -MF$(IntermediateDirectory)/happypr$(DependSuffix) -MM "/home/paris/.codelite/CSpace/happypr/happypr.c"

$(IntermediateDirectory)/happypr$(PreprocessSuffix): happypr.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/happypr$(PreprocessSuffix) "/home/paris/.codelite/CSpace/happypr/happypr.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/happypr$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/happypr$(DependSuffix)
	$(RM) $(IntermediateDirectory)/happypr$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/happypr"


