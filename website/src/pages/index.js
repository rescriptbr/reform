import React from "react";
import clsx from "clsx";
import Layout from "@theme/Layout";
import Link from "@docusaurus/Link";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import useBaseUrl from "@docusaurus/useBaseUrl";
import styles from "./styles.module.css";
console.log(styles);
export default function Home() {
  const context = useDocusaurusContext();
  const { siteConfig = {} } = context;
  return (
    <Layout
      title={`ReForm | Reasonably making forms sound good`}
      description="Reasonably making forms sound good"
    >
      <main className={clsx(styles["main-wrapper"])}>
        <img src="/static/img/hero-logo.svg" />
        <h1 className={styles['hero-title']}>
          Reasonably making forms with <span>ReScript</span> and <span>React</span> sound good
        </h1>
        <div>
          <Link
            className={clsx("button button--primary button--lg")}
            to={useBaseUrl("docs/")}
          >
            Get Started
          </Link>
        </div>
      </main>
    </Layout>
  );
}
