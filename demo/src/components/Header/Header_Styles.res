open CssJs

let logo = style(.[width(100->px)])

let header = style(.[
  display(#flex),
  justifyContent(#spaceBetween),
  padding2(~v=1.6->rem, ~h=zero),
  backgroundColor(Theme.Colors.black),
])

let navbar = style(.[
  width(100.0->pct),
  maxWidth(Theme.Constants.maxWidth),
  margin2(~v=zero, ~h=auto),
  display(#flex),
  alignItems(#center),
  justifyContent(#spaceBetween),
  selector(
    "> ul",
    [
      display(#flex),
      alignItems(#center),
      padding(zero),
      margin(zero),
      selector(
        "> li",
        [
          display(#flex),
          alignItems(#center),
          listStyleType(none),
          marginLeft(1.4->rem),
          selector(
            "> a",
            [
              fontSize(1.6->rem),
              cursor(pointer),
              fontWeight(#bold),
              textDecoration(none),
              color(white),
              opacity(0.8),
              transition(~duration=200, "opacity"),
              hover([opacity(1.0)]),
            ],
          ),
        ],
      ),
    ],
  ),
])

let activeLink = style(.[selector("> a", [opacity(1.0)->important])])
