# Uploading Project to GitHub

## Step 1: Install Git (if not installed)

### Option A: Using Windows Package Manager (Recommended)

```powershell
winget install Git.Git
```

After installation, **restart VS Code** to refresh PATH.

### Option B: Download from Website

1. Go to: https://git-scm.com/download/win
2. Download and install Git for Windows
3. Restart VS Code

### Verify Installation

Open a new terminal in VS Code and run:
```powershell
git --version
```

## Step 2: Initialize Git Repository

In VS Code terminal (in your project directory), run:

```powershell
# Initialize git repository
git init

# Configure your name and email (if not already set)
git config user.name "Your Name"
git config user.email "your.email@example.com"
```

## Step 3: Add Files to Git

```powershell
# Add all files (respects .gitignore)
git add .

# Check what will be committed
git status
```

## Step 4: Make Initial Commit

```powershell
git commit -m "Initial commit: Basic Placement Simulator"
```

## Step 5: Create GitHub Repository

1. Go to https://github.com
2. Click the **+** icon (top right) → **New repository**
3. Repository name: `basic-placement-simulator` (or your choice)
4. Description: "A simplified placement simulator for EDA that optimizes cell placement using simulated annealing"
5. Choose **Public** or **Private**
6. **DO NOT** initialize with README, .gitignore, or license (we already have these)
7. Click **Create repository**

## Step 6: Connect Local Repository to GitHub

After creating the repo, GitHub will show you commands. Use these:

```powershell
# Add remote repository (replace YOUR_USERNAME with your GitHub username)
git remote add origin https://github.com/YOUR_USERNAME/basic-placement-simulator.git

# Rename default branch to main (if needed)
git branch -M main

# Push to GitHub
git push -u origin main
```

## Step 7: Verify Upload

1. Go to your GitHub repository page
2. You should see all your files
3. The README.md will be displayed on the main page

## Quick Reference Commands

```powershell
# Initialize
git init
git add .
git commit -m "Initial commit"

# Connect to GitHub (replace with your repo URL)
git remote add origin https://github.com/YOUR_USERNAME/REPO_NAME.git
git branch -M main
git push -u origin main
```

## Future Updates

After making changes:

```powershell
git add .
git commit -m "Description of changes"
git push
```

## Troubleshooting

### "git is not recognized"
- Install Git (see Step 1)
- Restart VS Code after installation

### "Authentication failed"
- Use GitHub Personal Access Token instead of password
- Or use GitHub Desktop for easier authentication

### "Repository not found"
- Check the repository URL is correct
- Make sure the repository exists on GitHub
- Verify you have access to the repository

## Using GitHub Desktop (Alternative - Easier)

If you prefer a GUI:

1. Download GitHub Desktop: https://desktop.github.com/
2. Sign in with your GitHub account
3. File → Add Local Repository
4. Select your project folder
5. Click "Publish repository"
6. Choose name and visibility
7. Click "Publish Repository"

This handles authentication and setup automatically!

