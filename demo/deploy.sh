#!/bin/sh

GH_PAGES_BRANCH=gh-pages

TMP_DIR=$(mktemp -d)

echo "\033[1;33m What have you done to the site?\033[0m"
read update

# Create bundle
yarn
yarn build

# Copy build to temp folder
cp -r ./build/ $TMP_DIR/build
BUILD_FILES=`ls ./build`

cd ..

# Wipe out current site
git checkout $GH_PAGES_BRANCH
git rm -rf .
mv .git $TMP_DIR/.git
rm -rf .
mv $TMP_DIR/.git .git

# Move new bundle to master
mv -f $TMP_DIR/build/** ./

git add $BUILD_FILES

# Commit and push changes
git commit -m "$update"
echo "\033[1;31mPushing new site to GitHub repo\n\033[0m"
git push --force origin $GH_PAGES_BRANCH

# Get back to develop
git checkout develop
