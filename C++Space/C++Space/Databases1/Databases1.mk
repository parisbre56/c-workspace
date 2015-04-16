##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Databases1
ConfigurationName      :=Debug
WorkspacePath          := "/home/paris/.codelite/C++Space/C++Space"
ProjectPath            := "/home/paris/.codelite/C++Space/C++Space/Databases1"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=04/05/2013
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
ObjectsFileList        :="Databases1.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -m32 /home/paris/.codelite/C++Space/C++Space/Databases1/BF.o
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
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/main0$(ObjectSuffix) $(IntermediateDirectory)/DB1_Record$(ObjectSuffix) $(IntermediateDirectory)/DB1_Hash$(ObjectSuffix) $(IntermediateDirectory)/DB1_Heap$(ObjectSuffix) 



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
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Databases1/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/main0$(ObjectSuffix): main0.c $(IntermediateDirectory)/main0$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Databases1/main0.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main0$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main0$(DependSuffix): main0.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main0$(ObjectSuffix) -MF$(IntermediateDirectory)/main0$(DependSuffix) -MM "main0.c"

$(IntermediateDirectory)/main0$(PreprocessSuffix): main0.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main0$(PreprocessSuffix) "main0.c"

$(IntermediateDirectory)/DB1_Record$(ObjectSuffix): DB1_Record.cpp $(IntermediateDirectory)/DB1_Record$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Databases1/DB1_Record.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB1_Record$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB1_Record$(DependSuffix): DB1_Record.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB1_Record$(ObjectSuffix) -MF$(IntermediateDirectory)/DB1_Record$(DependSuffix) -MM "DB1_Record.cpp"

$(IntermediateDirectory)/DB1_Record$(PreprocessSuffix): DB1_Record.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB1_Record$(PreprocessSuffix) "DB1_Record.cpp"

$(IntermediateDirectory)/DB1_Hash$(ObjectSuffix): DB1_Hash.cpp $(IntermediateDirectory)/DB1_Hash$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Databases1/DB1_Hash.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB1_Hash$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB1_Hash$(DependSuffix): DB1_Hash.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB1_Hash$(ObjectSuffix) -MF$(IntermediateDirectory)/DB1_Hash$(DependSuffix) -MM "DB1_Hash.cpp"

$(IntermediateDirectory)/DB1_Hash$(PreprocessSuffix): DB1_Hash.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB1_Hash$(PreprocessSuffix) "DB1_Hash.cpp"

$(IntermediateDirectory)/DB1_Heap$(ObjectSuffix): DB1_Heap.cpp $(IntermediateDirectory)/DB1_Heap$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Databases1/DB1_Heap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/DB1_Heap$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/DB1_Heap$(DependSuffix): DB1_Heap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/DB1_Heap$(ObjectSuffix) -MF$(IntermediateDirectory)/DB1_Heap$(DependSuffix) -MM "DB1_Heap.cpp"

$(IntermediateDirectory)/DB1_Heap$(PreprocessSuffix): DB1_Heap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/DB1_Heap$(PreprocessSuffix) "DB1_Heap.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/main0$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main0$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main0$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Record$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Record$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Record$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Hash$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Hash$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Hash$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Heap$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Heap$(DependSuffix)
	$(RM) $(IntermediateDirectory)/DB1_Heap$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/Databases1"


