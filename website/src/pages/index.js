import React from "react";
import clsx from "clsx";
import Layout from "@theme/Layout";
import Link from "@docusaurus/Link";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import useBaseUrl from "@docusaurus/useBaseUrl";
import styles from "./styles.module.css";

export default function Home() {
  const context = useDocusaurusContext();
  const { siteConfig = {} } = context;
  return (
    <Layout
      title={`ReForm | Reasonably making forms sound good`}
      description="Reasonably making forms sound good"
    >
      <main>
        <div className={clsx(styles["main-wrapper"])}>
          <div className={clsx(styles.hero)}>
            <h1 className={styles["hero-title"]}>Powerful and type-safe</h1>
            <p className={styles["hero-text"]}>
              Reasonably making forms with <span>ReScript</span> and{" "}
              React sound good
            </p>
            <div>
              <Link
                className={clsx("button button--primary button--lg")}
                to={useBaseUrl("docs/")}
              >
                Get Started
              </Link>
            </div>
          </div>
        </div>
        <div>
          <div className="container text--center margin-top--xl margin-bottom--xl">
            <div className={clsx("row", styles.highlights)}>
              <section className="col margin-bottom--xl">
                <img src="/img/focus.svg" />
                <h2 className={styles["highlight-title"]}>Focused in ReScript</h2>
                <p className=" padding-horiz--md">
                  We created ReForm to be both deadly simple and to make forms
                  sound good leveraging ReScript's powerful typesytem. Even the
                  schemas we use are nothing more than constructors built-in in
                  the language itself with a small size footprint.
                </p>
              </section>
              <section className="col margin-bottom--xl">
                <img src="/img/powerful.svg" />
                <h2 className={styles["highlight-title"]}>Powerful and type-safe</h2>
                <p className=" padding-horiz--md">
                  ReForm is a powerful, type-safe and fully-featured form
                  library that includes hooks api, schema api, context provider,
                  field component and validation strategy.
                </p>
              </section>
              <section className="col margin-bottom--xl">
                <img src="/img/activity.svg" />
                <h2 className={styles["highlight-title"]}>Actively maintained</h2>
                <p className=" padding-horiz--md">
                  ReForm is used in production projects. This incentivizes us to
                  regularly improving the library and to keep breaking changes
                  to a minimum whereever possible.
                </p>
              </section>
            </div>
          </div>
        </div>
      </main>
    </Layout>
  );
}
