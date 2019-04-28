[@react.component]
let make = (~message) => {
  let url = ReasonReactRouter.useUrl();

  <ReasonApolloHooks.ApolloProvider client=Apollo.client>
    <div className="App">
      <div className="App-header">
        <img
          src="https://astrocoders.com/panel.png"
          className="App-logo"
          alt="logo"
        />
        <h2> {React.string(message)} </h2>
      </div>
      <h1> {ReasonReact.string("ReFormNext demo")} </h1>
      <a href="/new-post"> "New post form demo "->React.string </a>
      <br />
      <a href="/fav-colors"> "Form with array field demo"->React.string </a>
      <div style={ReactDOMRe.Style.make(~padding="100px", ())}>
        {switch (url.path) {
         | ["new-post"] => <PostAddNext />
         | ["fav-colors"] => <FavoriteColorsForm />
         | _ =>
           <p className="App-intro">
             {ReasonReact.string("Say hello to ReForm")}
           </p>
         }}
      </div>
    </div>
  </ReasonApolloHooks.ApolloProvider>;
};
