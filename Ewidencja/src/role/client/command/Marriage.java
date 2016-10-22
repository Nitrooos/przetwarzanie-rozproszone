package role.client.command;

import data.Database;
import data.Person;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Calendar;
import java.util.Optional;

public class Marriage implements Command {

    private Integer _id;
    private Person _person;

    public Marriage(Integer id, Person person) {
        this._id = id;
        this._person = person;
    }

    @Override
    public boolean run(Database db) {
        Optional<data.Marriage> maybe_marriage = this._person.getCurrentMarriage();
        if (maybe_marriage.isPresent()) {
            this.endMarriageConfirmation(db, maybe_marriage);
        } else {
            this.startMarriageConfirmation(db);
        }

        return true;
    }

    @Override
    public String name() {
        return "Zawrzyj/zakończ małżeństwo";
    }

    private void endMarriageConfirmation(Database db, Optional<data.Marriage> maybe) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        data.Marriage marriage = maybe.get();
        System.out.println("Czy na pewno chcesz zakończyć małżeństwo między " +
                marriage.getHusband().getFullName() + " i " +
                marriage.getWife().getFullName() + "? (T lub N): ");

        try {
            String answer = bufferedReader.readLine();
            if (answer.equals("T")) {
                marriage.end(Calendar.getInstance().getTime());
                db.update(this._id, this._person);

                Integer spouseId;
                if (this._person.getFullName().equals(marriage.getHusband().getFullName())) {
                    spouseId = db.findId(marriage.getWife());
                    db.update(spouseId, marriage.getWife());
                } else {
                    spouseId = db.findId(marriage.getHusband());
                    db.update(spouseId, marriage.getHusband());
                }

                System.out.println("Małżeństwo zostało zakończone\n");
            } else {
                System.out.println("Papiery rozwodowe wycofane!\n");
            }
        } catch (IOException e) {
            System.out.println("Błąd obsługi wejścia-wyjścia: " + e.getMessage());
        }
    }

    private void startMarriageConfirmation(Database db) {
        InputStreamReader streamReader = new InputStreamReader(System.in);
        BufferedReader bufferedReader = new BufferedReader(streamReader);

        System.out.println("Podaj ID osoby, z którą " + this._person.getFullName() + " ma zawrzeć małżeństwo: ");
        try {
            String input = bufferedReader.readLine();
            Integer id = Integer.parseInt(input);
            Person potential_spouse = db.get(id);
            if (potential_spouse != null) {
                data.Marriage m = new data.Marriage(this._person, potential_spouse).start(Calendar.getInstance().getTime());
                this._person.addMarriage(m);
                potential_spouse.addMarriage(m);
                db.update(this._id, this._person);
                db.update(id, potential_spouse);
                System.out.println(this._person.getFullName() + " zawarł małżeństwo z " + potential_spouse.getFullName());
            } else {
                System.out.println("Nie znaleziono w bazie osoby o ID równym " + id);
            }
        } catch (IOException e) {
            System.out.println("Błąd obsługi wejścia-wyjścia: " + e.getMessage());
        }
    }
}
