package role.client.command;

import data.Database;
import data.Person;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Delete implements Command {
    @Override
    public boolean run(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        System.out.println("Podaj numer ID osoby, którą chcesz usunąć z bazy: ");
        try {
            String input = bufferedReader.readLine();
            Integer id = Integer.parseInt(input);
            Person deleted = db.delete(id);
            if (deleted != null) {
                System.out.println("Pomyślnie usunięto osobę o ID " + id + " z bazy danych\n");
            } else {
                System.out.println("Nie znaleziono osoby o ID " + id + " w bazie danych; nic nie usunięto\n");
            }
        } catch (Exception e) {
            System.out.println("Błąd we-wy");
        }

        return true;
    }

    @Override
    public String name() {
        return "Usuń osobę";
    }
}
