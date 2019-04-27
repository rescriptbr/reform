[@react.component]
let make = (~message) => {
  <ReasonApolloHooks.ApolloProvider client=Apollo.client>
    <div className="App">
      <div className="App-header">
        <img
          src="https://astrocoders.com/panel.png"
          className="App-logo"
          alt="logo"
        />
        <h2> {ReasonReact.string(message)} </h2>
      </div>
      <p className="App-intro">
        {ReasonReact.string("Say hello to ReForm")}
      </p>
      <h1> {ReasonReact.string("ReFormNext demo")} </h1>
      <PostAddNext />
    </div>
  </ReasonApolloHooks.ApolloProvider>;
};
