package role.client.command;

import data.Database;
import data.Person;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Map;

public class Search implements Command {
    @Override
    public boolean run(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        System.out.println("Podaj frazę, po której ma nastąpić wyszukiwanie: ");
        try {
            String query = bufferedReader.readLine();
            Map<Integer, Person> searched = db.search(query);
            Person.printTable(searched);
        } catch (Exception e) {
            System.out.println("Błąd we-wy");
        }

        return true;
    }

    @Override
    public String name() {
        return "Wyszukaj osobę";
    }
}
