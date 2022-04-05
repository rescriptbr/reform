const lightCodeTheme = require("prism-react-renderer/themes/github")
const darkCodeTheme = require("prism-react-renderer/themes/dracula");

module.exports = {
  title: "ReForm",
  tagline: "Reasonably making forms sound good",
  url: "https://rescript-reform.netlify.app/",
  baseUrl: "/",
  onBrokenLinks: "throw",
  onBrokenMarkdownLinks: "warn",
  favicon: "img/favicon.ico",
  organizationName: "@rescriptbr",
  projectName: "reform",
  themeConfig: {
    colorMode: {
      defaultMode: "light",
      disableSwitch: true,
    },
    navbar: {
      title: "",
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
          href: "https://github.com/rescriptbr/reform/blob/master/reform/CHANGELOG.md",
          label: "Changelog",
          position: "left",
        },
        {
          href: "https://github.com/rescriptbr/reform",
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
              href: "http://discord.com/invite/SSDMNYQ",
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
    prism: {
      theme: lightCodeTheme,
      darkTheme: darkCodeTheme,
    },
  },
  presets: [
    [
      "@docusaurus/preset-classic",
      {
        docs: {
          sidebarPath: require.resolve("./sidebars.js"),
          editUrl: "https://github.com/rescriptbr/reform/edit/master/website/",
        },
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      },
    ],
  ],
};
