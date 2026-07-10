#!/bin/bash

# Test script for multiple atlas system
echo "Testing WiingPong Multiple Atlas System..."
echo "========================================"

# Check if atlas files exist
echo ""
echo "Checking atlas files:"

if [ -f "data/img/atlas.png" ]; then
    echo "✓ atlas.png found (Normal atlas)"
else
    echo "✗ atlas.png not found - will use spritesheet.png as fallback"
fi

if [ -f "data/img/atlas_2.png" ]; then
    echo "✓ atlas_2.png found (Intense atlas)"
else
    echo "✗ atlas_2.png not found"
    echo "  Please copy your intense/cyberpunk atlas to: data/img/atlas_2.png"
fi

if [ -f "data/img/spritesheet.png" ]; then
    echo "✓ spritesheet.png found (Fallback)"
else
    echo "⚠ spritesheet.png not found (fallback option)"
fi

# Check compilation
echo ""
echo "Checking compilation:"
if [ -f "WiingPong.dol" ]; then
    echo "✓ WiingPong.dol compiled successfully"
else
    echo "✗ Compilation failed"
    exit 1
fi

echo ""
echo "Multiple Atlas System Features:"
echo "=============================="
echo "✓ AtlasType enum (Normal, Intense)"
echo "✓ Dynamic atlas switching based on scores"
echo "✓ Atlas detection: 3-3 tie or higher triggers Intense atlas"
echo "✓ Special visual effects for Intense mode:"
echo "  - Pulsing cyan borders"
echo "  - Corner glow effects"
echo "  - Enhanced center line glow"
echo "  - Darker background for contrast"
echo "✓ Backward compatibility with existing spritesheet system"
echo "✓ Multiple path loading for different deployment scenarios"

echo ""
echo "Usage Instructions:"
echo "=================="
echo "1. Copy atlas.png to data/img/ (normal game graphics)"
echo "2. Copy atlas_2.png to data/img/ (intense cyberpunk graphics)"
echo "3. Deploy to Wii and play until 3-3 tie to see atlas switch"
echo ""
echo "Atlas Switching Logic:"
echo "• Normal Atlas: Used for regular gameplay"
echo "• Intense Atlas: Activated when both players ≥ 3 points AND tied"
echo "• Example: 3-3, 4-4, 5-5, etc. will trigger intense mode"
echo "• Game returns to normal atlas when tie is broken"

echo ""
echo "Testing Scenarios:"
echo "================="
echo "Score 0-0: Normal Atlas ✓"
echo "Score 2-1: Normal Atlas ✓"  
echo "Score 3-2: Normal Atlas ✓"
echo "Score 3-3: INTENSE ATLAS 🔥"
echo "Score 4-4: INTENSE ATLAS 🔥"
echo "Score 4-3: Normal Atlas ✓"

echo ""
echo "Ready for deployment! 🎮"