# Mining System Test Guide

This document provides test scenarios to verify the mining system is working correctly.

## Prerequisites

1. **Database Setup**: Ensure the database has been updated with the new mining fields:
   - `mining_level` (default: 1)
   - `mining_exp` (default: 0) 
   - `mining_exp_tnl` (default: 100)

2. **Configuration**: Verify `config/mining.ini` is loaded and contains mining weapon definitions and item configurations.

3. **Map Setup**: Ensure test maps have Unknown Tile 2 tiles placed for mining locations.

## Test Scenarios

### Test 1: Basic Mining Setup Verification

**Objective**: Verify mining configuration is loaded correctly.

**Steps**:
1. Start the server
2. Check server logs for mining configuration loading
3. Verify no errors related to mining.ini

**Expected Result**: Server starts without mining-related errors.

### Test 2: Mining Weapon Detection

**Objective**: Verify mining weapons are correctly identified.

**Steps**:
1. Create a character
2. Give the character a mining weapon (item ID 1, 2, or 3 as configured)
3. Equip the mining weapon
4. Attack a regular tile (should work normally)
5. Attack Unknown Tile 2 (should attempt mining)

**Expected Result**: 
- Regular attacks work normally
- Attacking Unknown Tile 2 with mining weapon triggers mining attempt

### Test 3: Mining Success and Failure

**Objective**: Test mining chance mechanics.

**Steps**:
1. Equip a mining weapon
2. Attack Unknown Tile 2 multiple times
3. Observe success/failure rates
4. Check inventory for mined items

**Expected Result**:
- Some mining attempts succeed (based on configured chances)
- Some mining attempts fail
- Successful attempts add items to inventory

### Test 4: Level Requirements

**Objective**: Verify level requirements prevent low-level mining.

**Steps**:
1. Create a level 1 character with mining level 1
2. Try to mine high-level items (level requirement > 1)
3. Level up mining skill
4. Try mining the same items again

**Expected Result**:
- Low-level characters cannot mine high-level items
- Higher-level characters can mine appropriate items

### Test 5: Experience Rewards

**Objective**: Test mining and regular EXP rewards.

**Steps**:
1. Note current mining EXP and regular EXP
2. Successfully mine an item
3. Check EXP gains
4. Verify level-ups work correctly

**Expected Result**:
- Mining EXP increases according to configuration
- Regular EXP increases if configured
- Level-ups trigger correctly

### Test 6: Announcements

**Objective**: Test mining success announcements.

**Steps**:
1. Successfully mine various items
2. Check for announcement messages
3. Verify message variables are replaced correctly

**Expected Result**:
- Announcements appear for successful mining
- Variables like {ITEM_NAME}, {MINING_EXP} are replaced correctly

### Test 7: Non-Mining Weapon

**Objective**: Verify non-mining weapons don't trigger mining.

**Steps**:
1. Equip a regular weapon (not configured as mining weapon)
2. Attack Unknown Tile 2
3. Verify no mining occurs

**Expected Result**: No mining attempt occurs with non-mining weapons.

### Test 8: Inventory Full

**Objective**: Test behavior when inventory is full.

**Steps**:
1. Fill inventory completely
2. Attempt mining
3. Check if mining still gives EXP without item

**Expected Result**: Mining may fail if inventory is full, but should handle gracefully.

## Admin Commands for Testing

If admin commands are available, use these for testing:

```
#item [mining_weapon_id] 1    // Give mining weapon
#warp [map_with_mining_tiles] // Go to mining area  
#setlevel mining [level]      // Set mining level (if supported)
#item [item_id] -999          // Remove items to test inventory space
```

## Configuration Testing

### Test Different Configurations

1. **High Success Rate**: Set item chance to 90% and test
2. **Low Success Rate**: Set item chance to 5% and test  
3. **Level Requirements**: Test items with various level requirements
4. **EXP Rewards**: Test different EXP reward amounts
5. **Announcements**: Test custom announcement messages

### Sample Test Configuration

Add this to `config/mining.ini` for testing:

```ini
# Test item with high success rate
999.chance = 90
999.level_req = 1
999.mining_exp = 50
999.regular_exp = 25
999.announcement = [TEST] You mined a test item! {MINING_EXP} Mining EXP gained!
```

## Expected Behavior Summary

1. **Mining Weapons**: Only configured weapons can mine
2. **Mining Tiles**: Only Unknown Tile 2 triggers mining
3. **Success Rates**: Based on configured percentages
4. **Level Requirements**: Items require minimum mining level
5. **EXP Rewards**: Both mining and regular EXP as configured
6. **Announcements**: Custom messages with variable replacement
7. **Level-ups**: Mining level increases with EXP (100 + level*50 formula)
8. **Database**: All mining data persists correctly

## Troubleshooting

**No Mining Occurs**:
- Check if weapon is configured as mining weapon
- Verify attacking Unknown Tile 2
- Check mining level requirements

**No EXP Gained**:
- Verify mining.ini configuration
- Check for configuration loading errors
- Ensure database fields exist

**No Announcements**:
- Check EnableAnnouncements setting
- Verify announcement format configuration
- Check for message parsing errors
