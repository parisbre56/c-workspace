.PHONY: clean All

All:
	@echo "----------Building project:[ Dtabases2 - Debug ]----------"
	@cd "Dtabases2" && $(MAKE) -f  "Dtabases2.mk"
clean:
	@echo "----------Cleaning project:[ Dtabases2 - Debug ]----------"
	@cd "Dtabases2" && $(MAKE) -f  "Dtabases2.mk" clean
