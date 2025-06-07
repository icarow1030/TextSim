import React from 'react';
import type { Message, LogEntry } from '../../App';
import ServerView from '../views/Server/ServerView';
import ChatView from '../views/Chat/ChatView';
import LogView from '../views/Log/LogView';
import ConnectionSettingsView from '../views/ConnSettings/ConnectionSettingsView'; // Importe o novo componente
import UserSettingsView from '../views/UserSettings/UserSettingsView'; // Importe o novo componente
import ManualView from '../views/Manual/ManualView'; // Importe o novo componente
import './MainContent.css';

interface MainContentProps {
  activeView: string;
  isServerOnline: boolean;
  toggleServer: () => void;
  messages: Message[];
  sendMessage: (message: string) => void;
  logs: LogEntry[];
  deleteAllLogs: () => void;
  // Adicione a prop addCustomLog se estiver usando
}

const MainContent: React.FC<MainContentProps> = (props) => {
  const renderContent = () => {
    switch (props.activeView) {
      case 'server':
        return <ServerView isOnline={props.isServerOnline} onToggle={props.toggleServer} />;
      case 'chat':
        return <ChatView messages={props.messages} onSendMessage={props.sendMessage} />;
      case 'logs':
        return (
          <LogView
            logs={props.logs}
            deleteAllLogs={props.deleteAllLogs}
            // addCustomLog={props.addCustomLog} // Passe se estiver usando
          />
        );
      // ADICIONE ESTES DOIS NOVOS CASES
      case 'connection':
        return <ConnectionSettingsView />;
      case 'user':
        return <UserSettingsView />;
      case 'manual':
        return <ManualView />; // Importe e use o componente ManualView
      case 'exit':
        return <h2>Saindo...</h2>;
      default:
        return <h2>Bem-vindo ao TextSim</h2>;
    }
  };

  return <main className="main-content">{renderContent()}</main>;
};

export default MainContent;