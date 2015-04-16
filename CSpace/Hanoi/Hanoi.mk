##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Hanoi
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/Hanoi"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=31/03/2012
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
ObjectsFileList        :="/home/paris/.codelite/CSpace/Hanoi/Hanoi.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
CmpOptions             := -g -O0 -Wall $(Preprocessors)
C_CmpOptions           := -g -O0 -Wall $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/Hanoi$(ObjectSuffix) $(IntermediateDirectory)/ch4_Stack$(ObjectSuffix) $(IntermediateDirectory)/TStoixeioyStoivas$(ObjectSuffix) 

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
$(IntermediateDirectory)/Hanoi$(ObjectSuffix): Hanoi.c $(IntermediateDirectory)/Hanoi$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Hanoi/Hanoi.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Hanoi$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Hanoi$(DependSuffix): Hanoi.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Hanoi$(ObjectSuffix) -MF$(IntermediateDirectory)/Hanoi$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Hanoi/Hanoi.c"

$(IntermediateDirectory)/Hanoi$(PreprocessSuffix): Hanoi.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Hanoi$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Hanoi/Hanoi.c"

$(IntermediateDirectory)/ch4_Stack$(ObjectSuffix): ch4_Stack.c $(IntermediateDirectory)/ch4_Stack$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Hanoi/ch4_Stack.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/ch4_Stack$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ch4_Stack$(DependSuffix): ch4_Stack.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ch4_Stack$(ObjectSuffix) -MF$(IntermediateDirectory)/ch4_Stack$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Hanoi/ch4_Stack.c"

$(IntermediateDirectory)/ch4_Stack$(PreprocessSuffix): ch4_Stack.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ch4_Stack$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Hanoi/ch4_Stack.c"

$(IntermediateDirectory)/TStoixeioyStoivas$(ObjectSuffix): TStoixeioyStoivas.c $(IntermediateDirectory)/TStoixeioyStoivas$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Hanoi/TStoixeioyStoivas.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TStoixeioyStoivas$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TStoixeioyStoivas$(DependSuffix): TStoixeioyStoivas.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TStoixeioyStoivas$(ObjectSuffix) -MF$(IntermediateDirectory)/TStoixeioyStoivas$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Hanoi/TStoixeioyStoivas.c"

$(IntermediateDirectory)/TStoixeioyStoivas$(PreprocessSuffix): TStoixeioyStoivas.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TStoixeioyStoivas$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Hanoi/TStoixeioyStoivas.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/Hanoi$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Hanoi$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Hanoi$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/ch4_Stack$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/ch4_Stack$(DependSuffix)
	$(RM) $(IntermediateDirectory)/ch4_Stack$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeioyStoivas$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeioyStoivas$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeioyStoivas$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/Hanoi"


