#include "src/config.hpp"
#include "src/console.hpp"
#include <iostream>

int main()
{
    Console::Out("Testing Crafting Configuration Loading...");
    
    try 
    {
        Config crafting_config;
        crafting_config.Read("./config/crafting.ini");
        
        Console::Out("✓ Crafting config loaded successfully!");
        
        // Test default settings
        auto default_crafting_exp = crafting_config.find("DefaultCraftingEXP");
        if (default_crafting_exp != crafting_config.end())
        {
            Console::Out("✓ DefaultCraftingEXP: %d", static_cast<int>(default_crafting_exp->second));
        }
        else
        {
            Console::Wrn("✗ DefaultCraftingEXP not found");
        }
        
        auto default_regular_exp = crafting_config.find("DefaultRegularEXP");
        if (default_regular_exp != crafting_config.end())
        {
            Console::Out("✓ DefaultRegularEXP: %d", static_cast<int>(default_regular_exp->second));
        }
        else
        {
            Console::Wrn("✗ DefaultRegularEXP not found");
        }
        
        auto enable_announcements = crafting_config.find("EnableAnnouncements");
        if (enable_announcements != crafting_config.end())
        {
            Console::Out("✓ EnableAnnouncements: %s", static_cast<std::string>(enable_announcements->second).c_str());
        }
        else
        {
            Console::Wrn("✗ EnableAnnouncements not found");
        }
        
        // Test item-specific settings
        auto item1_crafting_exp = crafting_config.find("1.crafting_exp");
        if (item1_crafting_exp != crafting_config.end())
        {
            Console::Out("✓ Item 1 crafting EXP: %d", static_cast<int>(item1_crafting_exp->second));
        }
        else
        {
            Console::Wrn("✗ Item 1 crafting EXP not found");
        }
        
        auto item40_announcement = crafting_config.find("40.announcement");
        if (item40_announcement != crafting_config.end())
        {
            Console::Out("✓ Item 40 announcement: %s", static_cast<std::string>(item40_announcement->second).c_str());
        }
        else
        {
            Console::Wrn("✗ Item 40 announcement not found");
        }
        
        Console::Out("Configuration test completed!");
        return 0;
    }
    catch (std::exception& e)
    {
        Console::Wrn("✗ Failed to load crafting config: %s", e.what());
        return 1;
    }
}