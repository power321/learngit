# !bin/bash
cd ~/backup
echo "You are In Backup Dir"
mv backup* ~/old_backup
echo "Old Database are Moved to old_backup folder"
Now=$(date +"%d-%m-%Y--%H:%M:%S")
File=backup-$Now.sql
mysqldump -u user-name -p[=password] db-name > $File
echo "Your Database Backup Successfully Completed"
