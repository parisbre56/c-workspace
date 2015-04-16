.PHONY: clean All

All:
	@echo ----------Building project:[ AirportManagement - Debug ]----------
	@cd "AirportManagement" && "$(MAKE)" -f "AirportManagement.mk"
clean:
	@echo ----------Cleaning project:[ AirportManagement - Debug ]----------
	@cd "AirportManagement" && "$(MAKE)" -f "AirportManagement.mk" clean
