ALTER TABLE [accounts]
    ALTER COLUMN [password] VARCHAR(64) NOT NULL,
    ADD [password_version] INTEGER DEFAULT 1 NOT NULL;
