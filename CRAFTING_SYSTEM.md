# Advanced Crafting System

This document describes the advanced crafting system implementation with full configuration support.

## Overview

The advanced crafting system extends the existing shops.ini crafting functionality with:
- Experience progression (separate crafting levels 1-100)
- Configurable EXP rewards per item
- Custom announcement messages
- Database persistence
- Automatic level progression

## Configuration

### Main Config Files

The crafting system uses the following configuration files:

1. **`config/crafting.ini`** - Main crafting configuration
2. **`config/files.ini`** - File path definitions
3. **`config.ini`** - Includes crafting.ini

### Crafting Configuration (`config/crafting.ini`)

#### Global Settings

```ini
# Default EXP rewards if item-specific settings not found
DefaultCraftingEXP = 70
DefaultRegularEXP = 450

# Enable/disable announcements
EnableAnnouncements = true

# Default announcement format
AnnouncementFormat = [Crafting] You have successfully crafted {ITEM_NAME}, you have earned {CRAFTING_EXP} Crafting EXP, and {REGULAR_EXP} EXP!
```

#### Item-Specific Settings

```ini
# Format: item_id.setting = value

# Basic item (ID 1)
1.crafting_exp = 25
1.regular_exp = 150
1.announcement = [Crafting] You crafted a basic {ITEM_NAME}!

# Legendary item (ID 40)
40.crafting_exp = 500
40.regular_exp = 2000
40.announcement = [Crafting] *** LEGENDARY CRAFT *** {PLAYER_NAME} has crafted {ITEM_NAME}!
```

#### Message Variables

Available variables for announcement messages:
- `{ITEM_NAME}` - Name of the crafted item
- `{CRAFTING_EXP}` - Crafting EXP earned
- `{REGULAR_EXP}` - Regular EXP earned  
- `{PLAYER_NAME}` - Player's character name
- `{CRAFTING_LEVEL}` - Player's current crafting level

## Database Schema

The system adds three new fields to the `characters` table:

```sql
ALTER TABLE characters ADD COLUMN crafting_level INTEGER NOT NULL DEFAULT 1;
ALTER TABLE characters ADD COLUMN crafting_exp INTEGER NOT NULL DEFAULT 0;  
ALTER TABLE characters ADD COLUMN crafting_exp_tnl INTEGER NOT NULL DEFAULT 100;
```

## Crafting Progression

### Level Requirements
- **Level 1→2**: 100 EXP
- **Level 2→3**: 150 EXP  
- **Level 3→4**: 200 EXP
- **Level N→N+1**: 100 + (N × 50) EXP
- **Maximum Level**: 100

### EXP Categories
- **Basic Items**: 25-30 Crafting EXP, 150-200 Regular EXP
- **Intermediate**: 50-60 Crafting EXP, 350-400 Regular EXP
- **Advanced**: 100-120 Crafting EXP, 600-700 Regular EXP
- **Rare**: 200 Crafting EXP, 1000 Regular EXP
- **Legendary**: 500 Crafting EXP, 2000 Regular EXP

## Usage

### Setting Up Crafting Recipes

1. **Configure shops.ini** with crafting recipes:
```ini
# Blacksmith shop (vendor ID 1)
1.name = Blacksmith
1.craft = 100,200,5,201,3,0,0,0,0  # Craft item 100 with ingredients
```

2. **Configure crafting.ini** with EXP rewards:
```ini
# Custom rewards for item 100
100.crafting_exp = 75
100.regular_exp = 400
100.announcement = [Smithing] You've forged {ITEM_NAME}!
```

### Local Overrides

Create `config_local/crafting.ini` for server-specific settings:
```ini
# Override default values
DefaultCraftingEXP = 100
EnableAnnouncements = false

# Custom legendary rewards
40.crafting_exp = 750
```

## System Integration

### Automatic Detection
- System automatically detects when players complete crafts from shops.ini
- No changes needed to existing crafting recipes
- Works with any NPC vendor ID and item combination

### Reward Processing
1. Player crafts item using existing shops.ini recipe
2. System calculates EXP rewards from crafting.ini
3. Awards crafting EXP and handles level progression
4. Awards regular EXP bonus
5. Sends customized announcement message
6. Saves all progress to database

### Error Handling
- Missing config file → Uses hardcoded defaults
- Missing item settings → Uses global defaults  
- Invalid values → Graceful fallbacks
- Config errors logged but don't crash server

## Administration

### Balancing Rewards
- Edit `config/crafting.ini` to adjust EXP values
- Restart server to reload configuration
- Use item-specific settings for fine-tuning

### Monitoring
- Server logs config loading status
- Debug mode shows EXP calculation details
- Database tracks all crafting progression

### Customization Examples

#### Themed Announcements
```ini
# Alchemy theme
100.announcement = [Alchemy] You've brewed a potent {ITEM_NAME}!
101.announcement = [Alchemy] Your {ITEM_NAME} bubbles with magical energy!

# Smithing theme  
200.announcement = [Smithing] The forge rings as you complete {ITEM_NAME}!
201.announcement = [Smithing] Your hammer strikes true, creating {ITEM_NAME}!
```

#### Progressive Rewards
```ini
# Beginner items (low rewards)
1.crafting_exp = 10
2.crafting_exp = 15

# Expert items (high rewards)
50.crafting_exp = 200
51.crafting_exp = 250
```

## Troubleshooting

### Config Not Loading
1. Check file path in `config/files.ini`
2. Verify `config.ini` includes crafting.ini
3. Check server console for error messages
4. Ensure proper file permissions

### EXP Not Awarded
1. Verify crafting recipe exists in shops.ini
2. Check crafting.ini has correct item ID
3. Enable debug mode for detailed logging
4. Confirm database has crafting fields

### Announcements Not Showing
1. Check `EnableAnnouncements = true` in config
2. Verify announcement format is valid
3. Test with default announcement format
4. Check for message variable syntax errors

## Technical Details

### Files Modified
- `install.sql` - Database schema
- `src/character.hpp/cpp` - Crafting methods and data
- `src/world.hpp/cpp` - Config loading
- `src/handlers/Shop.cpp` - Crafting detection
- `config/crafting.ini` - Configuration file
- `config/files.ini` - File paths
- `config.ini` - Config includes

### Performance Impact
- Minimal overhead per craft
- Database save only on craft completion
- Config loaded once at server startup
- No impact on regular gameplay

The advanced crafting system provides a flexible, configurable foundation for server-specific crafting experiences while maintaining compatibility with existing EOSERV functionality.