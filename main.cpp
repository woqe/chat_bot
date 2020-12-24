#include <bot.h>
#include "Item.h"
#include <memory>
#include <libs/data/Item.h>
#include "interface.h"

#define TELEGRAM_TOKEN "1310089:AAGMyPUkpjBHMoXXUcbvSkBRYZn-jP-q6dM"
#define CHAT_ID -1001335337991

int main(){
    Table users("users");
    users.addCol("telegram_id", Table::TYPE::LONG);
    users.addCol("reputation", Table::TYPE::INT);
    users.addCol("last_move_time", Table::TYPE::LONG);
    users.addCol("full_name", Table::TYPE::STRING);
    Table winners("winners");
    winners.addCol("telegram_id", Table::TYPE::LONG);

    Bot bot(TELEGRAM_TOKEN);
    while(bot.waitMessages(120)){
        vector<Telegram::Message> messages = bot.getMessages();
        for(int i = 0; i < messages.size(); ++i){
            Telegram::Message& message = messages[i];
            if(message.getChatTelegramId() != CHAT_ID)
                continue;
            vector<int> iDS = users.getIDs();
            int user_id = -1;
            for(int i = iDS.size(); i != 0; --i){
                if(users.readLong(iDS[i - 1], "telegram_id") == message.getSenderTelegramId()){
                    user_id = iDS[i - 1];
                    break;
                }
            }
            if(user_id < 0){
                user_id = users.addRow();
                users.writeLong(user_id, "telegram_id", message.getSenderTelegramId());
            }

            string full_name;
            if(message.getSenderFirstName().size())
                full_name += message.getSenderFirstName();
            if(message.getSenderLastName().size())
                full_name += " " + message.getSenderLastName();
            if(message.getSenderUserName().size())
                full_name += "[@" + message.getSenderUserName() + "]";

            users.writeString(user_id, "full_name", full_name);

            if(message.isReply() && message.getText()[0] == '+' && message.getSenderTelegramId() != message.getReplySenderId() && message.getReplySenderUsername() != "pns_chat_bot"){
                int reply_user_id = -1;
                for(int i = iDS.size(); i != 0; --i){
                    if(users.readLong(iDS[i - 1], "telegram_id") == message.getReplySenderId()){
                        reply_user_id = iDS[i - 1];
                        break;
                    }
                }
                if(reply_user_id < 0){
                    reply_user_id = users.addRow();
                    users.writeLong(reply_user_id, "telegram_id", message.getReplySenderId());
                }

                string reply_full_name;
                if(message.getReplySenderFirstName().size())
                    reply_full_name += message.getReplySenderFirstName();
                if(message.getReplySenderLastName().size())
                    reply_full_name += " " + message.getReplySenderLastName();
                if(message.getReplySenderUsername().size())
                    reply_full_name += "[@" + message.getReplySenderUsername() + "]";

                users.writeString(reply_user_id, "full_name", reply_full_name);

                if(message.getDateMessage() - users.readLong(user_id, "last_move_time") <= 7){
                    bot.sendMessage(CHAT_ID, markdown_free(full_name)
                    + " *\\(" + to_string(users.readInt(user_id, "reputation")) + "\\)* помедленнее, *бро\\!*");
                }else{
                    int add_rep =  ceil(users.readInt(user_id, "reputation") * 0.05);
                    if(!add_rep)
                        add_rep = 1;
                    users.writeInt(reply_user_id, "reputation", users.readInt(reply_user_id, "reputation") + add_rep);
                    bot.sendMessage(CHAT_ID, markdown_free(full_name)
                                             + " *\\(" + to_string(users.readInt(user_id, "reputation")) + "\\)* повысил\\(а\\) репутацию "
                                             + markdown_free(reply_full_name)
                                             + " *\\(" + to_string(users.readInt(reply_user_id, "reputation")) + "\\)*");
                    users.writeLong(user_id, "last_move_time", message.getDateMessage());
                }
            }else{
                if(message.getText() == "/top@pns_chat_bot"){
                    vector<int> iDS = users.getSortedByDecrease("reputation", Table::INT);
                    string text = "Скидка *30%*:\n";
                    if(iDS.size())
                        text += "🤩" + markdown_free(users.readString(iDS[0], "full_name")) + " \\(" + to_string(users.readInt(iDS[0], "reputation")) + "\\)\n";
                    else
                        text += "свободно\n";
                    text += "Скидка *20%*:\n";
                    for(int i = 1; i < 4; ++i)
                        if(iDS.size() >= i + 1)
                            text += "😎" + markdown_free(users.readString(iDS[i], "full_name")) + " \\(" + to_string(users.readInt(iDS[i], "reputation")) + "\\)\n";
                        else
                            text += "свободно\n";
                    text += "Скидка *15%*:\n";
                    for(int i = 4; i < 10; ++i)
                        if(iDS.size() >= i + 1)
                            text += "🤜🏽🤛" + markdown_free(users.readString(iDS[i], "full_name")) + " \\(" + to_string(users.readInt(iDS[i], "reputation")) + "\\)\n";
                        else
                            text += "свободно\n";
                    bot.sendMessage(CHAT_ID, text);
                }else if(message.getText() == "/my@pns_chat_bot"){
                    vector<int> iDS = users.getSortedByDecrease("reputation", Table::INT);
                    for(int i = 0; i < iDS.size(); ++i){
                        if(user_id == iDS[i]){
                            if(i + 1 == 0)
                                bot.sendMessage(CHAT_ID, markdown_free(full_name) + "\\(" + to_string(users.readInt(user_id, "reputation")) + "\\) твоя скидка составляет *30%*");
                            else if(i >= 1 && i + 1 <= 3)
                                bot.sendMessage(CHAT_ID, markdown_free(full_name) + "\\(" + to_string(users.readInt(user_id, "reputation")) + "\\) твоя скидка составляет *20%*");
                            else if(i + 1 >= 4 && i + 1 <= 9)
                                bot.sendMessage(CHAT_ID, markdown_free(full_name) + "\\(" + to_string(users.readInt(user_id, "reputation")) + "\\) твоя скидка составляет *15%*");
                            else if(users.readInt(user_id, "reputation") >= 50)
                                bot.sendMessage(CHAT_ID, markdown_free(full_name) + "\\(" + to_string(users.readInt(user_id, "reputation")) + "\\) твоя скидка составляет *5%*\n\nЕсли ты не состоишь в топе и твоя репутация больше 49 - дается постоянная скидка 5% \\(+ бонусом это рекламное сообщение\\)");
                            else
                                bot.sendMessage(CHAT_ID, markdown_free(full_name) + "\\(" + to_string(users.readInt(user_id, "reputation")) + "\\) твоя скидка составляет *0%*\n\nЕсли ты не состоишь в топе и твоя репутация больше 49 - дается постоянная скидка 5%");

                        }
                    }
                }else if(message.getText() == "подарок" || message.getText() == "Подарок"){
                    vector<int> iDS = winners.getIDs();
                    bool found = false;
                    for(int i = 0; i < iDS.size(); ++i){
                        if(users.readLong(user_id, "telegram_id") == winners.readLong(iDS[i], "telegram_id")){
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        int res = rand() % 100; // 0 - 99
                        if(res > 90){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 50% на весь заказ\\!*");
                        }else if(res > 80){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Бесплатный стимулятор Восход 0\\.5Л при заказе от 3000₽\\!*");
                        }else if(res > 70){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 40% на весь заказ\\!*");
                        }else if(res > 60){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Бесплатный стимулятор Восход 0\\.25Л при заказе от 3000₽\\!*");
                        }else if(res > 50){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 30% на весь заказ\\!*");
                        }else if(res > 40){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Бесплатный стимулятор Восход 0\\.5Л при заказе от 5000₽\\!*");
                        }else if(res > 30){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 20% на весь заказ\\!*");
                        }else if(res > 20){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Бесплатный стимулятор Восход 0\\.25Л при заказе от 5000₽\\!*");
                        }else if(res > 10){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 15% на весь заказ\\!*");
                        }else if(res > 0){
                            bot.sendMessage(CHAT_ID, "Хоу хоу хоу\\!\n" + markdown_free(full_name) + ", твой подарок \\- *Скидка 10% на весь заказ\\!*");
                        }
                        int winner_id = winners.addRow();
                        winners.writeLong(winner_id, "telegram_id", users.readLong(user_id, "telegram_id"));
                    }
                }
            }
        }
        users.save();
    }
}