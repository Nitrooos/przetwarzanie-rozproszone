import role.client.Client;
import role.Server;

import java.rmi.RMISecurityManager;

public class Main {

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("You have to give 'server' or 'client' as first argument!");
            return;
        }

        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new RMISecurityManager());
        }

        switch (args[0]) {
            case "server":
            {
                Server s = new Server();
                s.run();
                break;
            }
            case "client":
            {
                Client c = new Client();
                c.run();
                break;
            }
            default:
                break;
        }
    }

}
