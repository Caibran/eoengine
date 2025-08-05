# Mining System Debug Guide

The mining system has been implemented with debug output to help troubleshoot issues.

## Current Configuration

Based on your settings, the system should work as follows:

- **Mining Weapon**: Item ID 12
- **Mining Target**: Item ID 8 (75% chance, level 1 requirement)
- **Mining Tile**: Unknown Tile 2 (special tile)

## Debug Messages

When you attempt mining, you should see these debug messages:

### 1. Attack Handler Debug
When attacking any non-walkable tile:
```
DEBUG: Attacking non-walkable tile at X,Y
```

If it's Unknown Tile 2:
```
DEBUG: Detected Unknown Tile 2, attempting mining
```

If it's not Unknown Tile 2:
```
DEBUG: Not Unknown Tile 2, tile spec is [number]
```

### 2. Mining Attempt Debug
When mining is attempted:
```
DEBUG: Mining attempt started
DEBUG: Tile spec is [number], Unknown2 is [number]
```

If not a mining tile:
```
DEBUG: Not a mining tile
```

### 3. Weapon Check Debug
```
DEBUG: Weapon ID is [weapon_id]
DEBUG: Checking if weapon [weapon_id] is a mining weapon
DEBUG: Found mining weapon config: [config_weapon_id]
```

If weapon matches:
```
DEBUG: Weapon matches!
DEBUG: Mining weapon check passed
```

If no match:
```
DEBUG: Checked [count] mining weapon configs, no match found
DEBUG: No mining weapon equipped or weapon not valid for mining
```

## Testing Steps

1. **Start the server** with the updated code
2. **Create a character** and log in
3. **Give yourself item ID 12** (mining weapon)
4. **Equip item ID 12** as a weapon
5. **Find or create a map** with Unknown Tile 2 tiles
6. **Attack the Unknown Tile 2** and observe debug messages

## Expected Debug Flow

For a successful mining attempt, you should see:
```
DEBUG: Attacking non-walkable tile at X,Y
DEBUG: Detected Unknown Tile 2, attempting mining
DEBUG: Mining attempt started
DEBUG: Tile spec is 11, Unknown2 is 11
DEBUG: Weapon ID is 12
DEBUG: Checking if weapon 12 is a mining weapon
DEBUG: Found mining weapon config: 12
DEBUG: Weapon matches!
DEBUG: Mining weapon check passed
[Additional mining logic debug messages]
```

## Troubleshooting

### Issue: No debug messages at all
- Check if the server compiled successfully
- Verify you're attacking tiles (not empty space)

### Issue: "Not Unknown Tile 2" message
- The tile you're attacking is not Unknown Tile 2
- Use map editor to place Unknown Tile 2 tiles
- Unknown Tile 2 should have tile spec value 11

### Issue: "No mining weapon equipped"
- Verify item ID 12 is equipped as weapon
- Check if mining.ini is being loaded
- Verify MiningWeapon = 12 is in the config

### Issue: "Checked 0 mining weapon configs"
- mining.ini is not being loaded properly
- Check config/files.ini has MiningFile setting
- Check config.ini includes mining.ini
- Check file permissions on mining.ini

## Map Editor Setup

To create mining locations:
1. Open your map in EO Map Editor
2. Select "Special Tiles" or "Tile Spec" mode
3. Place "Unknown 2" tiles where you want mining spots
4. Save the map

## Admin Commands for Testing

If you have admin access, use these commands:
```
#item 12 1          // Give mining weapon
#warp [map_id]      // Go to test map
#summon [item_id]   // Give items for testing
```

## Configuration Verification

Your current mining.ini should have:
```ini
MiningWeapon = 12

8.chance = 75
8.level_req = 1
8.mining_exp = 50
8.regular_exp = 25
```

## Next Steps

1. Test with debug output enabled
2. Share the debug messages you see
3. Once working, we can remove debug output
4. Add more mining items and weapons as needed

## Common Issues

- **Wrong tile type**: Make sure you're using Unknown Tile 2, not Unknown Tile 1 or other tiles
- **Config not loaded**: Check server startup logs for config loading errors
- **Wrong weapon**: Make sure item ID 12 is equipped and configured as mining weapon
- **Database issues**: Ensure mining fields exist in characters table

The debug output will help identify exactly where the issue is occurring.
