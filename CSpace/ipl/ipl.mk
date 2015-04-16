##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=ipl
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/ipl"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=05/02/2012
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
ObjectsFileList        :="/home/paris/.codelite/CSpace/ipl/ipl.txt"
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
Objects=$(IntermediateDirectory)/paris_ipli$(ObjectSuffix) $(IntermediateDirectory)/paris_iplicommands$(ObjectSuffix) $(IntermediateDirectory)/paris_iplieval$(ObjectSuffix) $(IntermediateDirectory)/paris_iplisig$(ObjectSuffix) $(IntermediateDirectory)/paris_iplistruct$(ObjectSuffix) $(IntermediateDirectory)/paris_iplistring$(ObjectSuffix) 

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
$(IntermediateDirectory)/paris_ipli$(ObjectSuffix): ../../../ipli.c $(IntermediateDirectory)/paris_ipli$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/ipli.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_ipli$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_ipli$(DependSuffix): ../../../ipli.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_ipli$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_ipli$(DependSuffix) -MM "/home/paris/ipli.c"

$(IntermediateDirectory)/paris_ipli$(PreprocessSuffix): ../../../ipli.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_ipli$(PreprocessSuffix) "/home/paris/ipli.c"

$(IntermediateDirectory)/paris_iplicommands$(ObjectSuffix): ../../../iplicommands.c $(IntermediateDirectory)/paris_iplicommands$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/iplicommands.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_iplicommands$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_iplicommands$(DependSuffix): ../../../iplicommands.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_iplicommands$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_iplicommands$(DependSuffix) -MM "/home/paris/iplicommands.c"

$(IntermediateDirectory)/paris_iplicommands$(PreprocessSuffix): ../../../iplicommands.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_iplicommands$(PreprocessSuffix) "/home/paris/iplicommands.c"

$(IntermediateDirectory)/paris_iplieval$(ObjectSuffix): ../../../iplieval.c $(IntermediateDirectory)/paris_iplieval$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/iplieval.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_iplieval$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_iplieval$(DependSuffix): ../../../iplieval.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_iplieval$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_iplieval$(DependSuffix) -MM "/home/paris/iplieval.c"

$(IntermediateDirectory)/paris_iplieval$(PreprocessSuffix): ../../../iplieval.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_iplieval$(PreprocessSuffix) "/home/paris/iplieval.c"

$(IntermediateDirectory)/paris_iplisig$(ObjectSuffix): ../../../iplisig.c $(IntermediateDirectory)/paris_iplisig$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/iplisig.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_iplisig$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_iplisig$(DependSuffix): ../../../iplisig.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_iplisig$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_iplisig$(DependSuffix) -MM "/home/paris/iplisig.c"

$(IntermediateDirectory)/paris_iplisig$(PreprocessSuffix): ../../../iplisig.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_iplisig$(PreprocessSuffix) "/home/paris/iplisig.c"

$(IntermediateDirectory)/paris_iplistruct$(ObjectSuffix): ../../../iplistruct.c $(IntermediateDirectory)/paris_iplistruct$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/iplistruct.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_iplistruct$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_iplistruct$(DependSuffix): ../../../iplistruct.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_iplistruct$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_iplistruct$(DependSuffix) -MM "/home/paris/iplistruct.c"

$(IntermediateDirectory)/paris_iplistruct$(PreprocessSuffix): ../../../iplistruct.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_iplistruct$(PreprocessSuffix) "/home/paris/iplistruct.c"

$(IntermediateDirectory)/paris_iplistring$(ObjectSuffix): ../../../iplistring.c $(IntermediateDirectory)/paris_iplistring$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/iplistring.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/paris_iplistring$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/paris_iplistring$(DependSuffix): ../../../iplistring.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/paris_iplistring$(ObjectSuffix) -MF$(IntermediateDirectory)/paris_iplistring$(DependSuffix) -MM "/home/paris/iplistring.c"

$(IntermediateDirectory)/paris_iplistring$(PreprocessSuffix): ../../../iplistring.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/paris_iplistring$(PreprocessSuffix) "/home/paris/iplistring.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/paris_ipli$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_ipli$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_ipli$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplicommands$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplicommands$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplicommands$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplieval$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplieval$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplieval$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplisig$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplisig$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplisig$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistruct$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistruct$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistruct$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistring$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistring$(DependSuffix)
	$(RM) $(IntermediateDirectory)/paris_iplistring$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/ipl"


