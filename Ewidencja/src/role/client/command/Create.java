package role.client.command;

import data.BaseInfo;
import data.Database;
import data.Person;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Create implements Command {
    @Override
    public boolean run(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        try {
            System.out.println("Podaj imię: ");
            String firstname = bufferedReader.readLine();

            System.out.println("Podaj nazwisko: ");
            String lastname = bufferedReader.readLine();

            System.out.println("Podaj datę urodzenia (w formacie dd.mm.yyyy): ");
            String birth = bufferedReader.readLine();
            SimpleDateFormat dateFormat = new SimpleDateFormat("dd.MM.yyyy");
            Date birthDate = dateFormat.parse(birth);

            System.out.println("Podaj płeć (M lub K): ");
            String genderInput = bufferedReader.readLine();
            BaseInfo.Gender gender = BaseInfo.Gender.parse(genderInput);

            Person p = new Person(new BaseInfo(firstname, lastname, birthDate, gender));
            Person created = db.create(p);
            if (created != null) {
                System.out.println("Pomyślnie dodano nową osobę do bazy\n");
            } else {
                System.out.println("Wystąpiły błędy podczas przetwarzania; nic nie dodano do bazy\n");
            }
        } catch (Exception e) {
            System.out.println("Błąd we-wy: " + e.getMessage());
        }

        return true;
    }

    @Override
    public String name() {
        return "Dodaj nową osobę";
    }
}
