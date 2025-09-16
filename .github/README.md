# GitHub Actions CI/CD for WiingPong

This directory contains the GitHub Actions workflows and configurations for the WiingPong project.

## Workflows

### 🔧 CI/CD Pipeline (`ci.yml`)
**Triggers:** Push and PR to `master`/`develop` branches
- **Build Job:** Compiles both debug and release versions
- **Code Quality:** Static analysis and formatting checks
- **Artifacts:** Uploads build artifacts for download
- **DevkitPro:** Automatically sets up Wii development environment

### 🚀 Release (`release.yml`)
**Triggers:** Push to `master`, tags, or manual dispatch
- **Automated Releases:** Creates GitHub releases with archives
- **Release Notes:** Auto-generates changelog from commits
- **Packaging:** Creates `.tar.gz` and `.zip` distributions
- **Assets:** Includes DOL file, apps directory, and documentation

### ✅ PR Validation (`pr-validation.yml`)
**Triggers:** Pull request events
- **Build Validation:** Ensures both configurations compile
- **Git-Flow Compliance:** Validates branch naming conventions
- **Merge Conflicts:** Checks for conflicts before merge
- **Security Scan:** Basic security checks for common issues
- **Auto Comments:** Posts build status on PRs

### 🧹 Maintenance (`maintenance.yml`)
**Triggers:** Weekly schedule and manual dispatch
- **Dependency Updates:** Checks for DevkitPro updates
- **Artifact Cleanup:** Removes old build artifacts (30+ days)
- **Issue Creation:** Auto-creates issues for required updates

## Templates

### 📋 Issue Templates
- **Bug Report:** Structured bug reporting with Wii-specific fields
- **Feature Request:** Feature suggestions with complexity estimation

### 📝 Pull Request Template
- **Comprehensive Checklist:** Ensures quality and compliance
- **Git-Flow Validation:** Confirms proper branching strategy
- **Build Status:** Tracks compilation results
- **Testing Requirements:** Ensures thorough testing

## Configuration

### 📦 Dependabot (`dependabot.yml`)
- **GitHub Actions Updates:** Weekly updates for action dependencies
- **Auto-PRs:** Creates PRs for dependency updates
- **Labels:** Auto-labels dependency PRs

## Git-Flow Integration

The CI/CD pipeline is designed to work with git-flow:

```
master     ←── release/* ←── develop ←── feature/*
    ↑                          ↑
  hotfix/*                 copilot/*
```

### Branch Validation Rules:
- **PRs to `master`:** Should come from `release/*` or `hotfix/*`
- **PRs to `develop`:** Can come from `feature/*`, `hotfix/*`, or `copilot/*`
- **Automatic Releases:** Triggered on pushes to `master`

## DevkitPro Setup

The workflows automatically install and configure:
- DevkitPro package manager
- Wii development tools (`wii-dev`)
- PowerPC cross-compiler
- Required environment variables

## Build Artifacts

### Debug Builds
- Contains debug symbols and information
- Larger file size
- Used for development and debugging

### Release Builds  
- Optimized for production
- Smaller file size
- Ready for distribution

### Retention Policy
- **PR Artifacts:** 30 days
- **Release Artifacts:** 90 days
- **Old Artifacts:** Auto-cleaned after 30 days

## Security Features

- **Secret Scanning:** Checks for hardcoded credentials
- **Unsafe Functions:** Detects potentially unsafe C functions
- **Code Quality:** Basic static analysis
- **Dependency Updates:** Automated via Dependabot

## Manual Triggers

Some workflows support manual execution:
- **Release Workflow:** Create releases with custom version tags
- **Maintenance:** Run cleanup and update checks on-demand

## Environment Variables

Required in CI/CD:
- `DEVKITPRO=/opt/devkitpro`
- `DEVKITPPC=/opt/devkitpro/devkitPPC`
- `PATH` includes DevkitPro tools

## Troubleshooting

### Build Failures
1. Check DevkitPro installation logs
2. Verify Makefile configuration compatibility
3. Check for missing dependencies

### Artifact Issues
1. Verify file paths in workflow
2. Check retention policies
3. Ensure proper permissions

### Git-Flow Warnings
1. Review branch naming conventions
2. Confirm target branch is correct
3. Check for merge conflicts

## Contributing

When modifying workflows:
1. Test changes in a fork first
2. Follow YAML best practices
3. Update this documentation
4. Verify all environments (debug/release)

For more information, see the [Contributing Guide](../../CONTRIBUTING.md).
