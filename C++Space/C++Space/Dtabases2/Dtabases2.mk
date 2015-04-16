##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Dtabases2
ConfigurationName      :=Debug
WorkspacePath          := "/home/paris/.codelite/C++Space/C++Space"
ProjectPath            := "/home/paris/.codelite/C++Space/C++Space/Dtabases2"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=27/05/2013
CodeLitePath           :="/home/paris/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
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
ObjectsFileList        :="Dtabases2.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -m32 /home/paris/.codelite/C++Space/C++Space/Dtabases2/BF.o
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall  -m32 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  -m32 $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/DB2_DHeap$(ObjectSuffix) $(IntermediateDirectory)/DB2_Record$(ObjectSuffix) $(IntermediateDirectory)/DB2_CStore$(ObjectSuffix) $(IntermediateDirectory)/DB2_CSHT$(ObjectSuffix) $(IntermediateDirectory)/DB2_DHash$(ObjectSuffix) 



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
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/DB2_DHeap$(ObjectSuffix): DB2_DHeap.cpp $(IntermediateDirectory)/DB2_DHeap$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/DB2_DHeap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB2_DHeap$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB2_DHeap$(DependSuffix): DB2_DHeap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB2_DHeap$(ObjectSuffix) -MF$(IntermediateDirectory)/DB2_DHeap$(DependSuffix) -MM "DB2_DHeap.cpp"

$(IntermediateDirectory)/DB2_DHeap$(PreprocessSuffix): DB2_DHeap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB2_DHeap$(PreprocessSuffix) "DB2_DHeap.cpp"

$(IntermediateDirectory)/DB2_Record$(ObjectSuffix): DB2_Record.cpp $(IntermediateDirectory)/DB2_Record$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/DB2_Record.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB2_Record$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB2_Record$(DependSuffix): DB2_Record.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB2_Record$(ObjectSuffix) -MF$(IntermediateDirectory)/DB2_Record$(DependSuffix) -MM "DB2_Record.cpp"

$(IntermediateDirectory)/DB2_Record$(PreprocessSuffix): DB2_Record.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB2_Record$(PreprocessSuffix) "DB2_Record.cpp"

$(IntermediateDirectory)/DB2_CStore$(ObjectSuffix): DB2_CStore.cpp $(IntermediateDirectory)/DB2_CStore$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/DB2_CStore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB2_CStore$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB2_CStore$(DependSuffix): DB2_CStore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB2_CStore$(ObjectSuffix) -MF$(IntermediateDirectory)/DB2_CStore$(DependSuffix) -MM "DB2_CStore.cpp"

$(IntermediateDirectory)/DB2_CStore$(PreprocessSuffix): DB2_CStore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB2_CStore$(PreprocessSuffix) "DB2_CStore.cpp"

$(IntermediateDirectory)/DB2_CSHT$(ObjectSuffix): DB2_CSHT.cpp $(IntermediateDirectory)/DB2_CSHT$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/DB2_CSHT.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB2_CSHT$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB2_CSHT$(DependSuffix): DB2_CSHT.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB2_CSHT$(ObjectSuffix) -MF$(IntermediateDirectory)/DB2_CSHT$(DependSuffix) -MM "DB2_CSHT.cpp"

$(IntermediateDirectory)/DB2_CSHT$(PreprocessSuffix): DB2_CSHT.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB2_CSHT$(PreprocessSuffix) "DB2_CSHT.cpp"

$(IntermediateDirectory)/DB2_DHash$(ObjectSuffix): DB2_DHash.cpp $(IntermediateDirectory)/DB2_DHash$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Dtabases2/DB2_DHash.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB2_DHash$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB2_DHash$(DependSuffix): DB2_DHash.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB2_DHash$(ObjectSuffix) -MF$(IntermediateDirectory)/DB2_DHash$(DependSuffix) -MM "DB2_DHash.cpp"

$(IntermediateDirectory)/DB2_DHash$(PreprocessSuffix): DB2_DHash.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB2_DHash$(PreprocessSuffix) "DB2_DHash.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHeap$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHeap$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHeap$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB2_Record$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB2_Record$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB2_Record$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CStore$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CStore$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CStore$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CSHT$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CSHT$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB2_CSHT$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHash$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHash$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB2_DHash$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/Dtabases2"


