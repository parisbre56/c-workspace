##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Phonedata
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/Phonedata"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=23/05/2012
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
ObjectsFileList        :="/home/paris/.codelite/CSpace/Phonedata/Phonedata.txt"
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
Objects=$(IntermediateDirectory)/MobileCallManagement$(ObjectSuffix) $(IntermediateDirectory)/OuraPlus$(ObjectSuffix) $(IntermediateDirectory)/TStoixeiouOuraPlus$(ObjectSuffix) 

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
$(IntermediateDirectory)/MobileCallManagement$(ObjectSuffix): MobileCallManagement.c $(IntermediateDirectory)/MobileCallManagement$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Phonedata/MobileCallManagement.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/MobileCallManagement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MobileCallManagement$(DependSuffix): MobileCallManagement.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MobileCallManagement$(ObjectSuffix) -MF$(IntermediateDirectory)/MobileCallManagement$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Phonedata/MobileCallManagement.c"

$(IntermediateDirectory)/MobileCallManagement$(PreprocessSuffix): MobileCallManagement.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MobileCallManagement$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Phonedata/MobileCallManagement.c"

$(IntermediateDirectory)/OuraPlus$(ObjectSuffix): OuraPlus.c $(IntermediateDirectory)/OuraPlus$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Phonedata/OuraPlus.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/OuraPlus$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/OuraPlus$(DependSuffix): OuraPlus.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/OuraPlus$(ObjectSuffix) -MF$(IntermediateDirectory)/OuraPlus$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Phonedata/OuraPlus.c"

$(IntermediateDirectory)/OuraPlus$(PreprocessSuffix): OuraPlus.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/OuraPlus$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Phonedata/OuraPlus.c"

$(IntermediateDirectory)/TStoixeiouOuraPlus$(ObjectSuffix): TStoixeiouOuraPlus.c $(IntermediateDirectory)/TStoixeiouOuraPlus$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/Phonedata/TStoixeiouOuraPlus.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TStoixeiouOuraPlus$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TStoixeiouOuraPlus$(DependSuffix): TStoixeiouOuraPlus.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TStoixeiouOuraPlus$(ObjectSuffix) -MF$(IntermediateDirectory)/TStoixeiouOuraPlus$(DependSuffix) -MM "/home/paris/.codelite/CSpace/Phonedata/TStoixeiouOuraPlus.c"

$(IntermediateDirectory)/TStoixeiouOuraPlus$(PreprocessSuffix): TStoixeiouOuraPlus.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TStoixeiouOuraPlus$(PreprocessSuffix) "/home/paris/.codelite/CSpace/Phonedata/TStoixeiouOuraPlus.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/MobileCallManagement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/MobileCallManagement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/MobileCallManagement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/OuraPlus$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/OuraPlus$(DependSuffix)
	$(RM) $(IntermediateDirectory)/OuraPlus$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeiouOuraPlus$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeiouOuraPlus$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TStoixeiouOuraPlus$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/Phonedata"


