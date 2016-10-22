package data.seed;

import data.BaseInfo;
import data.Database;
import data.Person;

import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;

public class Seeder {
    public static void run(Database db) {
        List<Person> originals = new ArrayList();

        try {
            for (int i = 0; i < 3; ++i) {
                Person husband = new Person(BaseInfoGenerator.generate(BaseInfo.Gender.MALE, 70)),
                        wife = new Person(BaseInfoGenerator.generate(BaseInfo.Gender.FEMALE, 65));
                db.create(husband);
                db.create(wife);
                //            Marriage m = new Marriage(husband, wife).start(new Date());
            }
        } catch (RemoteException e) {
            System.out.println("Nastąpił błąd podczas seedowania bazy danych");
        }
    }
}
