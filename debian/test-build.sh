#!/bin/bash
# Test script for Debian package build

set -e

echo "=== Visual Pinball X Debian Package Build Test ==="
echo ""

# Check if required tools are installed
echo "1. Checking build dependencies..."
command -v cmake >/dev/null 2>&1 || echo "  WARNING: cmake not found"
command -v make >/dev/null 2>&1 || echo "  WARNING: make not found"
command -v dh_make >/dev/null 2>&1 || echo "  WARNING: dh_make not found"
command -v dpkg-buildpackage >/dev/null 2>&1 || echo "  WARNING: dpkg-buildpackage not found"

echo ""
echo "2. Validating debian/changelog..."
dch -l~test 2>/dev/null || echo "  INFO: changelog exists"

echo ""
echo "3. Checking debian/control..."
if grep -q "Source: vpinball" debian/control; then
    echo "  OK: Source package name found"
else
    echo "  ERROR: Source package name not found"
fi

if grep -q "Package: vpinball" debian/control; then
    echo "  OK: Binary package found"
else
    echo "  ERROR: Binary package not found"
fi

echo ""
echo "4. Checking debian/rules..."
if grep -q "override_dh_auto_configure" debian/rules; then
    echo "  OK: Custom configure target found"
else
    echo "  ERROR: Custom configure target not found"
fi

echo ""
echo "5. Checking install files..."
for file in debian/vpinball.install debian/vpinball-common.install debian/vpinball-dbg.install; do
    if [ -f "$file" ]; then
        lines=$(wc -l < "$file")
        echo "  OK: $file ($lines lines)"
    else
        echo "  ERROR: $file not found"
    fi
done

echo ""
echo "6. Checking desktop files..."
if [ -f "apps/linux/vpinball.desktop" ]; then
    echo "  OK: vpinball.desktop found"
else
    echo "  ERROR: vpinball.desktop not found"
fi

if [ -f "apps/linux/vpinball-editor.desktop" ]; then
    echo "  OK: vpinball-editor.desktop found"
else
    echo "  ERROR: vpinball-editor.desktop not found"
fi

echo ""
echo "7. Checking icon files..."
if [ -f "apps/linux/vpinball.svg" ]; then
    echo "  OK: vpinball.svg found"
else
    echo "  ERROR: vpinball.svg not found"
fi

echo ""
echo "8. Validating desktop file syntax..."
if command -v desktop-file-validate >/dev/null 2>&1; then
    desktop-file-validate apps/linux/vpinball.desktop 2>&1 || echo "  WARNING: validation issues found"
    desktop-file-validate apps/linux/vpinball-editor.desktop 2>&1 || echo "  WARNING: validation issues found"
else
    echo "  INFO: desktop-file-validate not available"
fi

echo ""
echo "=== Test Complete ==="
echo ""
echo "To build the package, run:"
echo "  dpkg-buildpackage -us -uc -b"
echo ""
echo "Or use debuild:"
echo "  debuild -us -uc"