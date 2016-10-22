package role;

import data.Database;
import data.PeopleDatabase;
import data.seed.Seeder;

import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;

public class Server extends Role {

    @Override
    public void run() {
        System.out.println("Server here");

        try {
            Database db = new PeopleDatabase();
            LocateRegistry.createRegistry(1099);
            Naming.rebind(this.getSharedBufferName(), db);
            Seeder.run(db);
        } catch (Exception e) {
            System.err.println("Server exception " + e.getMessage());
            e.printStackTrace();
        }
    }
}
