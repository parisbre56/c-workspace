##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AirportManagement
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/paris/.codelite/CSpace"
ProjectPath            := "/home/paris/.codelite/CSpace/AirportManagement"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=04/06/2012
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
ObjectsFileList        :="/home/paris/.codelite/CSpace/AirportManagement/AirportManagement.txt"
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
Objects=$(IntermediateDirectory)/AirportManagement$(ObjectSuffix) $(IntermediateDirectory)/Evretirio$(ObjectSuffix) $(IntermediateDirectory)/TSDDA$(ObjectSuffix) $(IntermediateDirectory)/TSEvr$(ObjectSuffix) $(IntermediateDirectory)/AVL_ch8_AVLpointer$(ObjectSuffix) $(IntermediateDirectory)/BST_ch8_BSTpointer$(ObjectSuffix) 

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
$(IntermediateDirectory)/AirportManagement$(ObjectSuffix): AirportManagement.c $(IntermediateDirectory)/AirportManagement$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/AirportManagement.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/AirportManagement$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AirportManagement$(DependSuffix): AirportManagement.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AirportManagement$(ObjectSuffix) -MF$(IntermediateDirectory)/AirportManagement$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/AirportManagement.c"

$(IntermediateDirectory)/AirportManagement$(PreprocessSuffix): AirportManagement.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AirportManagement$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/AirportManagement.c"

$(IntermediateDirectory)/Evretirio$(ObjectSuffix): Evretirio.c $(IntermediateDirectory)/Evretirio$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/Evretirio.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/Evretirio$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Evretirio$(DependSuffix): Evretirio.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Evretirio$(ObjectSuffix) -MF$(IntermediateDirectory)/Evretirio$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/Evretirio.c"

$(IntermediateDirectory)/Evretirio$(PreprocessSuffix): Evretirio.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Evretirio$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/Evretirio.c"

$(IntermediateDirectory)/TSDDA$(ObjectSuffix): TSDDA.c $(IntermediateDirectory)/TSDDA$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/TSDDA.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TSDDA$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TSDDA$(DependSuffix): TSDDA.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TSDDA$(ObjectSuffix) -MF$(IntermediateDirectory)/TSDDA$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/TSDDA.c"

$(IntermediateDirectory)/TSDDA$(PreprocessSuffix): TSDDA.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TSDDA$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/TSDDA.c"

$(IntermediateDirectory)/TSEvr$(ObjectSuffix): TSEvr.c $(IntermediateDirectory)/TSEvr$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/TSEvr.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/TSEvr$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TSEvr$(DependSuffix): TSEvr.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TSEvr$(ObjectSuffix) -MF$(IntermediateDirectory)/TSEvr$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/TSEvr.c"

$(IntermediateDirectory)/TSEvr$(PreprocessSuffix): TSEvr.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TSEvr$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/TSEvr.c"

$(IntermediateDirectory)/AVL_ch8_AVLpointer$(ObjectSuffix): AVL/ch8_AVLpointer.c $(IntermediateDirectory)/AVL_ch8_AVLpointer$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/AVL/ch8_AVLpointer.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/AVL_ch8_AVLpointer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AVL_ch8_AVLpointer$(DependSuffix): AVL/ch8_AVLpointer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AVL_ch8_AVLpointer$(ObjectSuffix) -MF$(IntermediateDirectory)/AVL_ch8_AVLpointer$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/AVL/ch8_AVLpointer.c"

$(IntermediateDirectory)/AVL_ch8_AVLpointer$(PreprocessSuffix): AVL/ch8_AVLpointer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AVL_ch8_AVLpointer$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/AVL/ch8_AVLpointer.c"

$(IntermediateDirectory)/BST_ch8_BSTpointer$(ObjectSuffix): BST/ch8_BSTpointer.c $(IntermediateDirectory)/BST_ch8_BSTpointer$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/paris/.codelite/CSpace/AirportManagement/BST/ch8_BSTpointer.c" $(C_CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/BST_ch8_BSTpointer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BST_ch8_BSTpointer$(DependSuffix): BST/ch8_BSTpointer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BST_ch8_BSTpointer$(ObjectSuffix) -MF$(IntermediateDirectory)/BST_ch8_BSTpointer$(DependSuffix) -MM "/home/paris/.codelite/CSpace/AirportManagement/BST/ch8_BSTpointer.c"

$(IntermediateDirectory)/BST_ch8_BSTpointer$(PreprocessSuffix): BST/ch8_BSTpointer.c
	@$(C_CompilerName) $(C_CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BST_ch8_BSTpointer$(PreprocessSuffix) "/home/paris/.codelite/CSpace/AirportManagement/BST/ch8_BSTpointer.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/AirportManagement$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AirportManagement$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AirportManagement$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/Evretirio$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/Evretirio$(DependSuffix)
	$(RM) $(IntermediateDirectory)/Evretirio$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TSDDA$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TSDDA$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TSDDA$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TSEvr$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TSEvr$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TSEvr$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/AVL_ch8_AVLpointer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/AVL_ch8_AVLpointer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/AVL_ch8_AVLpointer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/BST_ch8_BSTpointer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/BST_ch8_BSTpointer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/BST_ch8_BSTpointer$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/paris/.codelite/CSpace/.build-debug/AirportManagement"


