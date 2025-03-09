@echo off
git add .
git commit -m "Updated files"
git pull origin main --rebase
git push origin main
echo Done!
pause
