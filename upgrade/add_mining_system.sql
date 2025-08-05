-- Mining System Database Upgrade
-- This script adds mining system fields to existing character tables
-- Run this on existing databases to add mining support

-- Add mining fields to characters table
ALTER TABLE `characters` 
ADD COLUMN `mining_level` INTEGER NOT NULL DEFAULT 1 AFTER `crafting_exp_tnl`,
ADD COLUMN `mining_exp` INTEGER NOT NULL DEFAULT 0 AFTER `mining_level`,
ADD COLUMN `mining_exp_tnl` INTEGER NOT NULL DEFAULT 100 AFTER `mining_exp`;

-- Verify the changes
SELECT 
    COLUMN_NAME, 
    DATA_TYPE, 
    IS_NULLABLE, 
    COLUMN_DEFAULT 
FROM INFORMATION_SCHEMA.COLUMNS 
WHERE TABLE_NAME = 'characters' 
    AND COLUMN_NAME IN ('mining_level', 'mining_exp', 'mining_exp_tnl')
ORDER BY ORDINAL_POSITION;
