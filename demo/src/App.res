open Render

@react.component
let make = () => {
  let url = RescriptReactRouter.useUrl()
  <Layout>
    <Header />
    <Layout.Content>
      {switch url.path {
      | list{"basic-usage"} => <BasicUsage />
      | list{"sign-up"} => <SignUp />
      | list{"todo-list"} => <div> {str("Todo List...")} </div>
      | _ => <SignUp />
      }}
    </Layout.Content>
    <Layout.Footer />
  </Layout>
}
