#Commit message
param([string]$message='none')

git add .
git commit -m $message
git push
