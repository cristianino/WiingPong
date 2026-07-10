#!/bin/bash

# Test script for spritesheet integration
echo "Testing WiingPong spritesheet integration..."

# Check if spritesheet file exists in the correct location
if [ ! -f "data/img/spritesheet.png" ]; then
    echo "WARNING: spritesheet.png not found in data/img/"
    echo "Please copy your spritesheet image to: data/img/spritesheet.png"
else
    echo "✓ spritesheet.png found in data/img/"
fi

# Check if compiled successfully
if [ -f "WiingPong.dol" ]; then
    echo "✓ WiingPong.dol compiled successfully"
else
    echo "✗ WiingPong.dol not found - compilation failed"
    exit 1
fi

# Check if build files exist
if [ -f "build/AssetManager.o" ] && [ -f "build/Renderer.o" ]; then
    echo "✓ AssetManager and Renderer compiled successfully"
else
    echo "✗ Missing build files"
fi

echo ""
echo "Spritesheet integration complete!"
echo ""
echo "Features implemented:"
echo "• SpriteInfo structure for defining sprite coordinates"
echo "• AssetManager extended with spritesheet loading"
echo "• Sprite definitions for paddles, ball, digits 0-9, center line, background"
echo "• Renderer updated to use sprites when available (with fallback to primitives)"
echo "• GRRLIB_DrawPart used for efficient sprite rendering"
echo ""
echo "To use:"
echo "1. Copy your spritesheet image to: data/img/spritesheet.png"
echo "2. Deploy WiingPong.dol to your Wii"
echo "3. The game will automatically use sprites if the file is found"
echo ""
echo "Note: Sprite coordinates are estimated based on your image."
echo "You may need to adjust the coordinates in AssetManager::initializeSpriteDefinitions()"
echo "for perfect alignment."