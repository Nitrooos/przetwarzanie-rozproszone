package role.client.command;

import data.BaseInfo;
import data.Database;
import data.Person;

import java.rmi.RemoteException;
import java.text.SimpleDateFormat;
import java.util.Map;

public class ListAll implements Command {

    @Override
    public boolean run(Database db) {
        try {
            Map<Integer, Person> people = db.getAll();
            Person.printTable(people);
        } catch (RemoteException e) {
            e.printStackTrace();
            System.out.println("Nastąpił błąd w komunikacji z serwerem");
        }
        return true;
    }

    @Override
    public String name() {
        return "Wylistuj wszystkich";
    }
}
