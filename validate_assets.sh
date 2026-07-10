#!/bin/bash

# Asset validation script for WiingPong
echo "🔍 Validating WiingPong Assets..."
echo "================================"

error_count=0

# Function to check if file is a valid PNG
check_png() {
    local file="$1"
    local name="$2"
    
    if [ -f "$file" ]; then
        file_type=$(file "$file" | grep -o "PNG image data")
        if [ -n "$file_type" ]; then
            echo "✅ $name: Valid PNG image"
            file "$file" | grep -o "[0-9]* x [0-9]*"
        else
            echo "❌ $name: NOT a valid PNG image!"
            echo "   File type: $(file "$file")"
            ((error_count++))
        fi
    else
        echo "⚠️  $name: File not found (optional)"
    fi
}

echo ""
echo "Checking image files:"
check_png "data/img/atlas.png" "Normal Atlas"
check_png "data/img/atlas_2.png" "Intense Atlas" 
check_png "data/img/spritesheet.png" "Legacy Spritesheet"
check_png "data/img/wiipong-banner.png" "Banner Image"

echo ""
echo "Checking audio files:"
for audio_file in data/sounds/*.pcm; do
    if [ -f "$audio_file" ]; then
        echo "🔊 $(basename "$audio_file"): $(file "$audio_file" | cut -d: -f2)"
    fi
done

echo ""
echo "Checking compiled output:"
if [ -f "WiingPong.dol" ]; then
    echo "✅ WiingPong.dol: $(file WiingPong.dol | cut -d: -f2)"
else
    echo "❌ WiingPong.dol: Missing! Run 'make' to compile."
    ((error_count++))
fi

echo ""
if [ $error_count -eq 0 ]; then
    echo "🎉 All assets validated successfully!"
    echo "✅ Ready for deployment"
else
    echo "⚠️  Found $error_count error(s)"
    echo "❌ Fix errors before deployment"
fi

echo ""
echo "Quick fixes:"
echo "• If spritesheet.png is text: cp atlas.png spritesheet.png"
echo "• If atlas missing: copy your PNG files to data/img/"
echo "• If compilation failed: make clean && make"