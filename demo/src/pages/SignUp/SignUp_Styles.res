open CssJs

let form = style(.[maxWidth(400->px), margin2(~v=32->px, ~h=auto)])

let wrapper = style(.[
  //
  height(100.0->pct),
  display(#flex),
  alignItems(center),
  justifyContent(center),
])
