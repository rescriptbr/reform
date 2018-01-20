/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* List of projects/orgs using your project for the users page */
const users = [
  {
    caption: 'Astrocoders',
    image: 'https://astrocoders.com/images/hero/logo.png',
    infoLink: 'https://astrocoders.com',
    pinned: true,
  },
];

const siteConfig = {
  title: 'ReForm' /* title for your website */,
  tagline: 'Making forms sound good',
  url: 'https://astrocoders.com' /* your website url */,
  baseUrl: '/reform/' /* base url for your project */,
  projectName: 'reform',
  headerLinks: [
    {doc: 'installation', label: 'Docs'},
    {doc: 'schema', label: 'API'},
    {page: 'help', label: 'Help'},
    {blog: true, label: 'Blog'},
  ],
  users,
  /* path to images for header/footer */
  headerIcon: 'img/reform.svg',
  footerIcon: 'img/reform.svg',
  favicon: 'img/favicon.png',
  /* colors for website */
  colors: {
    primaryColor: '#CF5954',
    secondaryColor: '#85CAAE',
  },
  // This copyright info is used in /core/Footer.js and blog rss/atom feeds.
  copyright:
    'Copyright © ' +
    new Date().getFullYear() +
    'Astrocodres',
  organizationName: 'Astrocoders', // or set an env variable ORGANIZATION_NAME
  projectName: 'reform', // or set an env variable PROJECT_NAME
  highlight: {
    // Highlight.js theme to use for syntax highlighting in code blocks
    theme: 'default',
  },
  scripts: ['https://buttons.github.io/buttons.js'],
  // You may provide arbitrary config keys to be used as needed by your template.
  repoUrl: 'https://github.com/Astrocoders/reform',
};

module.exports = siteConfig;
