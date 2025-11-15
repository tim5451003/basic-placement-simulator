# Git setup script for GitHub upload
# Run this in VS Code terminal after restarting VS Code (to refresh PATH)

Write-Host "Setting up Git repository..." -ForegroundColor Cyan
Write-Host ""

# Refresh PATH
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Check if git is available
$git = Get-Command git -ErrorAction SilentlyContinue
if (-not $git) {
    Write-Host "ERROR: Git not found in PATH!" -ForegroundColor Red
    Write-Host "Please restart VS Code after installing Git." -ForegroundColor Yellow
    Write-Host "Then run this script again." -ForegroundColor Yellow
    exit 1
}

Write-Host "Found Git: $($git.Source)" -ForegroundColor Green
Write-Host ""

# Check if already a git repository
if (Test-Path ".git") {
    Write-Host "Git repository already initialized." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "To add and commit files:" -ForegroundColor Cyan
    Write-Host "  git add ." -ForegroundColor White
    Write-Host "  git commit -m 'Initial commit'" -ForegroundColor White
    Write-Host ""
    Write-Host "To connect to GitHub:" -ForegroundColor Cyan
    Write-Host "  git remote add origin https://github.com/YOUR_USERNAME/REPO_NAME.git" -ForegroundColor White
    Write-Host "  git branch -M main" -ForegroundColor White
    Write-Host "  git push -u origin main" -ForegroundColor White
    exit 0
}

# Initialize repository
Write-Host "Initializing git repository..." -ForegroundColor Cyan
git init

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Failed to initialize git repository!" -ForegroundColor Red
    exit 1
}

Write-Host "✓ Repository initialized" -ForegroundColor Green
Write-Host ""

# Check git config
Write-Host "Checking git configuration..." -ForegroundColor Cyan
$userName = git config user.name
$userEmail = git config user.email

if (-not $userName -or -not $userEmail) {
    Write-Host "Git user name/email not configured." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Please configure your git identity:" -ForegroundColor Cyan
    Write-Host "  git config user.name 'Your Name'" -ForegroundColor White
    Write-Host "  git config user.email 'your.email@example.com'" -ForegroundColor White
    Write-Host ""
} else {
    Write-Host "✓ Git configured: $userName <$userEmail>" -ForegroundColor Green
    Write-Host ""
}

# Show next steps
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Add files to git:" -ForegroundColor Yellow
Write-Host "   git add ." -ForegroundColor White
Write-Host ""
Write-Host "2. Make initial commit:" -ForegroundColor Yellow
Write-Host "   git commit -m 'Initial commit: Basic Placement Simulator'" -ForegroundColor White
Write-Host ""
Write-Host "3. Create repository on GitHub:" -ForegroundColor Yellow
Write-Host "   - Go to https://github.com" -ForegroundColor White
Write-Host "   - Click '+' → New repository" -ForegroundColor White
Write-Host "   - Name it (e.g., 'basic-placement-simulator')" -ForegroundColor White
Write-Host "   - DO NOT initialize with README" -ForegroundColor White
Write-Host ""
Write-Host "4. Connect and push:" -ForegroundColor Yellow
Write-Host "   git remote add origin https://github.com/YOUR_USERNAME/REPO_NAME.git" -ForegroundColor White
Write-Host "   git branch -M main" -ForegroundColor White
Write-Host "   git push -u origin main" -ForegroundColor White
Write-Host ""
Write-Host "See GITHUB_UPLOAD.md for detailed instructions." -ForegroundColor Gray

