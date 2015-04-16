##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Scene3
ConfigurationName      :=Debug
WorkspacePath          := "/home/paris/.codelite/C++Space/C++Space"
ProjectPath            := "/home/paris/.codelite/C++Space/C++Space/Scene3"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=21/02/2013
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
ObjectsFileList        :="Scene3.txt"
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
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/lake$(ObjectSuffix) $(IntermediateDirectory)/exceptions$(ObjectSuffix) $(IntermediateDirectory)/IceSkater$(ObjectSuffix) $(IntermediateDirectory)/pos2D$(ObjectSuffix) 

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
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Scene3/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/lake$(ObjectSuffix): lake.cpp $(IntermediateDirectory)/lake$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Scene3/lake.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lake$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lake$(DependSuffix): lake.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lake$(ObjectSuffix) -MF$(IntermediateDirectory)/lake$(DependSuffix) -MM "lake.cpp"

$(IntermediateDirectory)/lake$(PreprocessSuffix): lake.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lake$(PreprocessSuffix) "lake.cpp"

$(IntermediateDirectory)/exceptions$(ObjectSuffix): exceptions.cpp $(IntermediateDirectory)/exceptions$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Scene3/exceptions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/exceptions$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/exceptions$(DependSuffix): exceptions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/exceptions$(ObjectSuffix) -MF$(IntermediateDirectory)/exceptions$(DependSuffix) -MM "exceptions.cpp"

$(IntermediateDirectory)/exceptions$(PreprocessSuffix): exceptions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/exceptions$(PreprocessSuffix) "exceptions.cpp"

$(IntermediateDirectory)/IceSkater$(ObjectSuffix): IceSkater.cpp $(IntermediateDirectory)/IceSkater$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Scene3/IceSkater.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IceSkater$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IceSkater$(DependSuffix): IceSkater.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IceSkater$(ObjectSuffix) -MF$(IntermediateDirectory)/IceSkater$(DependSuffix) -MM "IceSkater.cpp"

$(IntermediateDirectory)/IceSkater$(PreprocessSuffix): IceSkater.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IceSkater$(PreprocessSuffix) "IceSkater.cpp"

$(IntermediateDirectory)/pos2D$(ObjectSuffix): pos2D.cpp $(IntermediateDirectory)/pos2D$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/Scene3/pos2D.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/pos2D$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/pos2D$(DependSuffix): pos2D.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/pos2D$(ObjectSuffix) -MF$(IntermediateDirectory)/pos2D$(DependSuffix) -MM "pos2D.cpp"

$(IntermediateDirectory)/pos2D$(PreprocessSuffix): pos2D.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/pos2D$(PreprocessSuffix) "pos2D.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/lake$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/lake$(DependSuffix)
	$(RM) $(IntermediateDirectory)/lake$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/exceptions$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/exceptions$(DependSuffix)
	$(RM) $(IntermediateDirectory)/exceptions$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/IceSkater$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/IceSkater$(DependSuffix)
	$(RM) $(IntermediateDirectory)/IceSkater$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/pos2D$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/pos2D$(DependSuffix)
	$(RM) $(IntermediateDirectory)/pos2D$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/Scene3"


