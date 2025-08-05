# Mining System Implementation

This document describes the mining system implementation for EOSERV, based on the EOSource-Revived approach but adapted for this codebase.

## Overview

The mining system allows players to use mining tools to extract resources from special mining tiles (Unknown Tile 2) on maps. Players gain mining experience and can level up their mining skill to access better resources.

## How It Works

1. **Mining Tools**: Players must equip a weapon configured as a mining tool
2. **Mining Locations**: Players attack Unknown Tile 2 tiles on maps
3. **Success Chance**: Each item has a configurable success chance percentage
4. **Level Requirements**: Items require minimum mining levels to obtain
5. **Experience Rewards**: Players gain mining EXP and optionally regular EXP
6. **Announcements**: Success messages are displayed to players

## Database Schema

The mining system adds three new fields to the `characters` table:

```sql
`mining_level`   INTEGER NOT NULL DEFAULT 1,
`mining_exp`     INTEGER NOT NULL DEFAULT 0,
`mining_exp_tnl` INTEGER NOT NULL DEFAULT 100,
```

### For Existing Installations

Run the upgrade script to add mining fields to existing databases:

```bash
mysql -u username -p database_name < upgrade/add_mining_system.sql
```

## Configuration

### Mining Configuration File

The mining system is configured in `config/mining.ini`. This file defines:

- Mining weapons (tools that can be used for mining)
- Mineable items with their chances, level requirements, and EXP rewards
- Global settings and announcement formats

### Example Configuration

```ini
# Global Settings
DefaultChance = 50
DefaultMiningEXP = 100
EnableAnnouncements = true
AnnouncementFormat = [MINING] You have successfully mined {ITEM_NAME}! You received {MINING_EXP} EXP!

# Mining Weapons
MiningWeapon = 1    # Pickaxe
MiningWeapon = 2    # Shovel
MiningWeapon = 3    # Mining Tool

# Mining Items
4.chance = 75           # 75% success chance
4.level_req = 1         # Requires mining level 1
4.mining_exp = 50       # Gives 50 mining EXP
4.regular_exp = 25      # Gives 25 regular EXP
4.announcement = [MINING] You mined {ITEM_NAME} and gained {MINING_EXP} Mining EXP!
```

### Configuration Options

#### Global Settings
- `DefaultChance`: Default success percentage if item-specific not set
- `DefaultMiningEXP`: Default mining EXP reward if item-specific not set
- `DefaultRegularEXP`: Default regular EXP reward if item-specific not set
- `EnableAnnouncements`: Enable/disable mining announcements
- `AnnouncementFormat`: Default announcement message template

#### Mining Weapons
- `MiningWeapon = [item_id]`: Define which weapon IDs can be used for mining

#### Item Configuration
For each mineable item, you can configure:
- `[item_id].chance = [percentage]`: Success chance (0-100)
- `[item_id].level_req = [level]`: Minimum mining level required
- `[item_id].mining_exp = [amount]`: Mining EXP reward
- `[item_id].regular_exp = [amount]`: Regular EXP reward (optional)
- `[item_id].announcement = [message]`: Custom announcement (optional)

#### Announcement Variables
Available variables for announcement messages:
- `{ITEM_NAME}`: Name of the mined item
- `{MINING_EXP}`: Mining EXP earned
- `{REGULAR_EXP}`: Regular EXP earned
- `{PLAYER_NAME}`: Player's character name
- `{MINING_LEVEL}`: Player's current mining level

## Map Setup

To create mining locations:

1. Open your map in the EO Map Editor
2. Place Unknown Tile 2 tiles where you want mining locations
3. Save the map
4. Players can now mine at these locations

## Level Progression

Mining levels use the same formula as crafting:
- Level 1 → 2: 100 EXP
- Level 2 → 3: 150 EXP (100 + 2*50)
- Level 3 → 4: 200 EXP (100 + 3*50)
- And so on...

Formula: `100 + (level * 50)` EXP required for next level.

## Code Structure

### Files Modified/Added

1. **config/mining.ini**: Mining configuration file
2. **src/character.hpp**: Added mining fields and method declarations
3. **src/character.cpp**: Implemented mining logic and methods
4. **src/world.hpp**: Added mining_config field
5. **src/world.cpp**: Added mining config loading
6. **src/map.cpp**: Modified attack handler to detect mining attempts
7. **install.sql**: Added mining fields to database schema
8. **config/files.ini**: Added MiningFile configuration
9. **lang/en.ini**: Added mining level-up message

### Key Methods

- `Character::AttemptMining()`: Main mining logic
- `Character::IsMiningWeapon()`: Check if weapon can mine
- `Character::GiveMiningEXP()`: Award mining experience
- `Character::CalculateMiningEXPReward()`: Calculate EXP rewards
- `Character::AnnounceMining()`: Send mining success messages

## Testing

See `test_mining.md` for comprehensive testing procedures.

### Quick Test

1. Start the server
2. Create a character and give them a mining weapon (item ID 1, 2, or 3)
3. Equip the mining weapon
4. Find a map with Unknown Tile 2 tiles
5. Attack the Unknown Tile 2
6. Check for mining success messages and inventory items

## Troubleshooting

### Common Issues

**Mining doesn't work:**
- Verify weapon is configured as mining weapon in mining.ini
- Ensure attacking Unknown Tile 2 tiles
- Check mining level requirements for items

**No EXP gained:**
- Check mining.ini configuration syntax
- Verify configuration file is loading without errors
- Ensure database fields exist

**No announcements:**
- Check `EnableAnnouncements = true` in mining.ini
- Verify announcement format configuration

### Debug Steps

1. Check server startup logs for mining config loading errors
2. Verify database schema includes mining fields
3. Test with high-chance items first (90%+ success rate)
4. Use admin commands to verify item IDs and weapon IDs

## Customization

### Adding New Mining Items

1. Add item configuration to mining.ini:
```ini
[item_id].chance = [percentage]
[item_id].level_req = [level]
[item_id].mining_exp = [exp_amount]
[item_id].announcement = [custom_message]
```

### Adding New Mining Weapons

1. Add weapon to mining.ini:
```ini
MiningWeapon = [weapon_item_id]
```

### Custom Announcements

Create unique messages for special items:
```ini
999.announcement = *** RARE FIND *** {PLAYER_NAME} discovered {ITEM_NAME}! {MINING_EXP} Mining EXP earned!
```

## Integration Notes

This mining system integrates seamlessly with the existing EOSERV codebase:

- Uses the same configuration system as other features
- Follows the same database patterns as crafting system
- Integrates with existing attack/combat system
- Uses standard packet builders for client communication
- Supports the same admin and debugging tools

The implementation is designed to be maintainable and extensible, following EOSERV coding conventions and patterns.
