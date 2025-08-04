# Bags Feature Implementation

## Overview
The bags feature has been successfully implemented as requested. This feature provides players with an instanced storage system separate from the bank lockers, with level-based capacity that can be upgraded through the quest system.

## Feature Breakdown

### 1. Database Schema Changes
- **File**: `install.sql`
- **Changes**:
  - Added `baglevel` INTEGER field (default: 1) to track player's bag upgrade level
  - Added `bag` TEXT field to store serialized bag contents

### 2. Character Data Structure
- **File**: `src/character.hpp`
- **Changes**:
  - Added `std::list<Character_Item> bag;` member to store bag items
  - Added `int baglevel;` member to track bag upgrade level

### 3. Character Data Persistence
- **Files**: `src/character.cpp`, `src/world.cpp`
- **Changes**:
  - Updated SELECT, INSERT, and UPDATE queries to include bag fields
  - Added bag data serialization/deserialization
  - Initialized baglevel to 1 for new characters

### 4. Player Command Implementation
- **File**: `src/commands/bag.cpp`
- **Functionality**: 
  - Implements the `#bag` command (accessible to all players - admin level 0)
  - Opens the bag interface using special coordinates (255, 255) to distinguish from regular lockers
  - Reuses the existing locker packet protocol for compatibility

### 5. Bag Storage System
- **File**: `src/handlers/Locker.cpp`
- **Functionality**:
  - Extended existing locker handlers to support bag operations
  - Bag capacity formula: 3 base slots + (baglevel - 1) additional slots
  - Uses special coordinates (255, 255) to identify bag operations vs locker operations
  - Instanced storage - no physical location requirements

### 6. Quest System Integration
- **File**: `src/quest.cpp`
- **Quest Action**: `givebagLevel <levels>`
  - Increases player's bag level by the specified amount
  - Minimum bag level is 1 (cannot go below)
  - Usage: `action givebagLevel 2` (gives 2 bag levels)
  
- **Quest Rule**: `HasBagSpace <required_slots>`
  - Checks if player has enough free bag space
  - Returns true if available space >= required slots
  - Usage: `rule HasBagSpace 3` (checks if player has 3+ free bag slots)

### 7. Admin Configuration
- **File**: `admin.ini`
- **Change**: Added `bag = 0` to allow all players to use the #bag command

## Technical Implementation Details

### Bag Capacity System
- **Level 1**: 3 slots (base)
- **Level 2**: 4 slots (3 + 1)
- **Level 3**: 5 slots (3 + 2)
- **Level N**: (3 + N - 1) slots

### Protocol Integration
- Reuses existing `PACKET_LOCKER` protocol for compatibility
- Uses coordinates (255, 255) as a special flag to identify bag operations
- Standard locker operations (ADD, TAKE, OPEN) work seamlessly with bags

### Data Storage
- Bag contents are serialized using the same format as inventory/bank items
- Bag level is stored as an integer in the database
- All data is properly persisted across login sessions

## Usage Examples

### For Players
```
#bag          // Opens the bag interface
```

### For Quest Designers
```
// Give player 2 additional bag levels
action givebagLevel 2

// Check if player has at least 1 free bag slot
rule HasBagSpace 1

// Example quest that gives bag upgrade if player has space
state "upgrade_bag":
    rule HasBagSpace 1
    action givebagLevel 1
    action addnpctext "Your bag has been upgraded!"
```

### For Administrators
The bag command is available to all players (admin level 0) by default, but this can be changed in `admin.ini` if needed.

## Files Modified/Created

### Modified Files:
1. `install.sql` - Database schema
2. `src/character.hpp` - Character class definition
3. `src/character.cpp` - Character data loading/saving
4. `src/world.cpp` - Character data export/import
5. `src/handlers/Locker.cpp` - Bag operation handlers
6. `src/quest.cpp` - Quest actions and rules
7. `admin.ini` - Command permissions

### Created Files:
1. `src/commands/bag.cpp` - Bag command implementation

## Compatibility Notes
- The implementation reuses existing locker packet protocols for maximum client compatibility
- No client-side changes are required
- The feature integrates seamlessly with the existing bank/locker system
- Database changes are backward compatible (new fields have default values)

## Testing Recommendations
1. Test bag opening with `#bag` command
2. Test adding/removing items to/from bag
3. Test bag capacity limits at different levels
4. Test quest actions: `givebagLevel` and rule: `HasBagSpace`
5. Test data persistence across character login/logout
6. Test that bag operations don't interfere with regular locker operations