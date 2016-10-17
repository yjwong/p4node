const p4node = require('../index');
const ui = new p4node.ClientUser();
const client = new p4node.ClientApi();
const e = new p4node.Error();

client.Init(e);
if (e.Test()) {
  // To do: Add error handling code.
  return 1;
}

client.SetArgv(0, []);
client.Run("info", ui);
client.Final(e);

if (e.Test()) {
  // To do: Add error handling code.
  return 1;
}
