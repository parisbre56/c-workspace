##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=L5R
ConfigurationName      :=Debug
WorkspacePath          := "/home/paris/.codelite/C++Space/C++Space"
ProjectPath            := "/home/paris/.codelite/C++Space/C++Space/L5R"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Paris
Date                   :=07/03/2013
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
ObjectsFileList        :="L5R.txt"
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
CXXFLAGS :=  -g -O0 -Wall -D__L5R_DEBUG_CERR__ $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/L5RCard$(ObjectSuffix) $(IntermediateDirectory)/L5RGameBoard$(ObjectSuffix) $(IntermediateDirectory)/L5RPlayer$(ObjectSuffix) $(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/L5RStronghold$(ObjectSuffix) $(IntermediateDirectory)/L5RGreenCard$(ObjectSuffix) $(IntermediateDirectory)/L5RBlackCard$(ObjectSuffix) $(IntermediateDirectory)/L5RItem$(ObjectSuffix) $(IntermediateDirectory)/L5RFollower$(ObjectSuffix) $(IntermediateDirectory)/L5RPersonality$(ObjectSuffix) \
	$(IntermediateDirectory)/L5RHolding$(ObjectSuffix) $(IntermediateDirectory)/L5RTypeConverter$(ObjectSuffix) $(IntermediateDirectory)/L5RDeckBuilder$(ObjectSuffix) $(IntermediateDirectory)/L5RProvince$(ObjectSuffix) 

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
$(IntermediateDirectory)/L5RCard$(ObjectSuffix): L5RCard.cpp $(IntermediateDirectory)/L5RCard$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RCard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RCard$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RCard$(DependSuffix): L5RCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RCard$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RCard$(DependSuffix) -MM "L5RCard.cpp"

$(IntermediateDirectory)/L5RCard$(PreprocessSuffix): L5RCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RCard$(PreprocessSuffix) "L5RCard.cpp"

$(IntermediateDirectory)/L5RGameBoard$(ObjectSuffix): L5RGameBoard.cpp $(IntermediateDirectory)/L5RGameBoard$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RGameBoard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RGameBoard$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RGameBoard$(DependSuffix): L5RGameBoard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RGameBoard$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RGameBoard$(DependSuffix) -MM "L5RGameBoard.cpp"

$(IntermediateDirectory)/L5RGameBoard$(PreprocessSuffix): L5RGameBoard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RGameBoard$(PreprocessSuffix) "L5RGameBoard.cpp"

$(IntermediateDirectory)/L5RPlayer$(ObjectSuffix): L5RPlayer.cpp $(IntermediateDirectory)/L5RPlayer$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RPlayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RPlayer$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RPlayer$(DependSuffix): L5RPlayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RPlayer$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RPlayer$(DependSuffix) -MM "L5RPlayer.cpp"

$(IntermediateDirectory)/L5RPlayer$(PreprocessSuffix): L5RPlayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RPlayer$(PreprocessSuffix) "L5RPlayer.cpp"

$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/L5RStronghold$(ObjectSuffix): L5RStronghold.cpp $(IntermediateDirectory)/L5RStronghold$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RStronghold.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RStronghold$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RStronghold$(DependSuffix): L5RStronghold.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RStronghold$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RStronghold$(DependSuffix) -MM "L5RStronghold.cpp"

$(IntermediateDirectory)/L5RStronghold$(PreprocessSuffix): L5RStronghold.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RStronghold$(PreprocessSuffix) "L5RStronghold.cpp"

$(IntermediateDirectory)/L5RGreenCard$(ObjectSuffix): L5RGreenCard.cpp $(IntermediateDirectory)/L5RGreenCard$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RGreenCard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RGreenCard$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RGreenCard$(DependSuffix): L5RGreenCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RGreenCard$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RGreenCard$(DependSuffix) -MM "L5RGreenCard.cpp"

$(IntermediateDirectory)/L5RGreenCard$(PreprocessSuffix): L5RGreenCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RGreenCard$(PreprocessSuffix) "L5RGreenCard.cpp"

$(IntermediateDirectory)/L5RBlackCard$(ObjectSuffix): L5RBlackCard.cpp $(IntermediateDirectory)/L5RBlackCard$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RBlackCard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RBlackCard$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RBlackCard$(DependSuffix): L5RBlackCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RBlackCard$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RBlackCard$(DependSuffix) -MM "L5RBlackCard.cpp"

$(IntermediateDirectory)/L5RBlackCard$(PreprocessSuffix): L5RBlackCard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RBlackCard$(PreprocessSuffix) "L5RBlackCard.cpp"

$(IntermediateDirectory)/L5RItem$(ObjectSuffix): L5RItem.cpp $(IntermediateDirectory)/L5RItem$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RItem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RItem$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RItem$(DependSuffix): L5RItem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RItem$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RItem$(DependSuffix) -MM "L5RItem.cpp"

$(IntermediateDirectory)/L5RItem$(PreprocessSuffix): L5RItem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RItem$(PreprocessSuffix) "L5RItem.cpp"

$(IntermediateDirectory)/L5RFollower$(ObjectSuffix): L5RFollower.cpp $(IntermediateDirectory)/L5RFollower$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RFollower.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RFollower$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RFollower$(DependSuffix): L5RFollower.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RFollower$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RFollower$(DependSuffix) -MM "L5RFollower.cpp"

$(IntermediateDirectory)/L5RFollower$(PreprocessSuffix): L5RFollower.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RFollower$(PreprocessSuffix) "L5RFollower.cpp"

$(IntermediateDirectory)/L5RPersonality$(ObjectSuffix): L5RPersonality.cpp $(IntermediateDirectory)/L5RPersonality$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RPersonality.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RPersonality$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RPersonality$(DependSuffix): L5RPersonality.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RPersonality$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RPersonality$(DependSuffix) -MM "L5RPersonality.cpp"

$(IntermediateDirectory)/L5RPersonality$(PreprocessSuffix): L5RPersonality.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RPersonality$(PreprocessSuffix) "L5RPersonality.cpp"

$(IntermediateDirectory)/L5RHolding$(ObjectSuffix): L5RHolding.cpp $(IntermediateDirectory)/L5RHolding$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RHolding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RHolding$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RHolding$(DependSuffix): L5RHolding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RHolding$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RHolding$(DependSuffix) -MM "L5RHolding.cpp"

$(IntermediateDirectory)/L5RHolding$(PreprocessSuffix): L5RHolding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RHolding$(PreprocessSuffix) "L5RHolding.cpp"

$(IntermediateDirectory)/L5RTypeConverter$(ObjectSuffix): L5RTypeConverter.cpp $(IntermediateDirectory)/L5RTypeConverter$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RTypeConverter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RTypeConverter$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RTypeConverter$(DependSuffix): L5RTypeConverter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RTypeConverter$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RTypeConverter$(DependSuffix) -MM "L5RTypeConverter.cpp"

$(IntermediateDirectory)/L5RTypeConverter$(PreprocessSuffix): L5RTypeConverter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RTypeConverter$(PreprocessSuffix) "L5RTypeConverter.cpp"

$(IntermediateDirectory)/L5RDeckBuilder$(ObjectSuffix): L5RDeckBuilder.cpp $(IntermediateDirectory)/L5RDeckBuilder$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RDeckBuilder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RDeckBuilder$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RDeckBuilder$(DependSuffix): L5RDeckBuilder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RDeckBuilder$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RDeckBuilder$(DependSuffix) -MM "L5RDeckBuilder.cpp"

$(IntermediateDirectory)/L5RDeckBuilder$(PreprocessSuffix): L5RDeckBuilder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RDeckBuilder$(PreprocessSuffix) "L5RDeckBuilder.cpp"

$(IntermediateDirectory)/L5RProvince$(ObjectSuffix): L5RProvince.cpp $(IntermediateDirectory)/L5RProvince$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/paris/.codelite/C++Space/C++Space/L5R/L5RProvince.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/L5RProvince$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/L5RProvince$(DependSuffix): L5RProvince.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/L5RProvince$(ObjectSuffix) -MF$(IntermediateDirectory)/L5RProvince$(DependSuffix) -MM "L5RProvince.cpp"

$(IntermediateDirectory)/L5RProvince$(PreprocessSuffix): L5RProvince.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/L5RProvince$(PreprocessSuffix) "L5RProvince.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/L5RCard$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RCard$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RCard$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RGameBoard$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RGameBoard$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RGameBoard$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RPlayer$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RPlayer$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RPlayer$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RStronghold$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RStronghold$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RStronghold$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RGreenCard$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RGreenCard$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RGreenCard$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RBlackCard$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RBlackCard$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RBlackCard$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RItem$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RItem$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RItem$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RFollower$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RFollower$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RFollower$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RPersonality$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RPersonality$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RPersonality$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RHolding$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RHolding$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RHolding$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RTypeConverter$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RTypeConverter$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RTypeConverter$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RDeckBuilder$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RDeckBuilder$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RDeckBuilder$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/L5RProvince$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/L5RProvince$(DependSuffix)
	$(RM) $(IntermediateDirectory)/L5RProvince$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../.build-debug/L5R"


