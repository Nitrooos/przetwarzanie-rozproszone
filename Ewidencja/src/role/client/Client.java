package role.client;

import data.Database;
import role.Role;
import role.client.command.*;

import java.rmi.Naming;
import java.util.TreeMap;

public class Client extends Role {

    private Menu _menu = new Menu(new TreeMap<Integer, Command>() {{
        put(1, new Create());
        put(2, new ListAll());
        put(3, new Search());
        put(4, new Show());
        put(5, new Delete());
        put(6, new Quit());
    }});

    @Override
    public void run() {
        System.out.println("Witamy w programie Ewidencja Ludności v0.1");

        try {
            Database db = (Database) Naming.lookup(this.getSharedBufferName());

            while (true) {
                this._menu.draw();
                Command selected = this._menu.readChoice();
                if (!selected.run(db)) {
                    break;
                }
            }
        } catch (Exception e) {
            System.err.println("Client exception " + e.getMessage());
        }
    }

}
