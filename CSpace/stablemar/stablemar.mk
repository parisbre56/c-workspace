##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=stablemar
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/stablemar"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=06/01/2012
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
ObjectsFileList        :="/home/paris/.codelite/CSpace/stablemar/stablemar.txt"
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
Objects=$(IntermediateDirectory)/stablemar$(ObjectSuffix) 

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
$(IntermediateDirectory)/stablemar$(ObjectSuffix): stablemar.c $(IntermediateDirectory)/stablemar$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/stablemar/stablemar.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/stablemar$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/stablemar$(DependSuffix): stablemar.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/stablemar$(ObjectSuffix) -MF$(IntermediateDirectory)/stablemar$(DependSuffix) -MM "/home/paris/.codelite/CSpace/stablemar/stablemar.c"

$(IntermediateDirectory)/stablemar$(PreprocessSuffix): stablemar.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/stablemar$(PreprocessSuffix) "/home/paris/.codelite/CSpace/stablemar/stablemar.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/stablemar$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/stablemar$(DependSuffix)
	$(RM) $(IntermediateDirectory)/stablemar$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/stablemar"


