/** @type {import('@docusaurus/types').DocusaurusConfig} */
module.exports = {
  title: "ReForm",
  tagline: "Reasonably making forms sound good",
  url: "https://reasonbr.org/reform",
  baseUrl: "/",
  onBrokenLinks: "throw",
  onBrokenMarkdownLinks: "warn",
  favicon: "img/favicon.ico",
  organizationName: "@reasonbr",
  projectName: "reform",
  themeConfig: {
    colorMode: {
      defaultMode: "light",
      disableSwitch: true,
    },
    navbar: {
      title: "ReForm",
      logo: {
        alt: "ReForm Logo",
        src: "img/logo.svg",
      },
      items: [
        {
          to: "docs/",
          activeBasePath: "docs",
          label: "Docs",
          position: "left",
        },
        {
          href:
            "https://github.com/reasonbr/reform/blob/master/packages/bs-reform/CHANGELOG.md",
          label: "Changelog",
          position: "left",
        },
        {
          href: "https://github.com/reasonbr/reform",
          label: "GitHub",
          position: "right",
        },
      ],
    },
    footer: {
      style: "dark",
      links: [
        {
          title: "Docs",
          items: [
            {
              label: "Installation",
              to: "docs/installation",
            },
            {
              label: "Getting started",
              to: "docs/getting-started",
            },
            {
              label: "Basic usage",
              to: "docs/basic-usage",
            },
          ],
        },
        {
          title: "Community",
          items: [
            {
              label: "Discord",
              href: "https://discordapp.com/invite/docusaurus",
            },
            {
              label: "Twitter",
              href: "https://twitter.com/reasonmlbrasil",
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} ReForm. Built with Docusaurus.`,
    },
  },
  presets: [
    [
      "@docusaurus/preset-classic",
      {
        docs: {
          sidebarPath: require.resolve("./sidebars.js"),
          editUrl: "https://github.com/reasonbr/reform/edit/master/website/",
        },
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      },
    ],
  ],
};
