// src/App.tsx
import { useState, useEffect } from 'react';
import Sidebar from './components/Sidebar/Sidebar';
import MainContent from './components/MainContent/MainContent';
import './App.css';

// Tipos para nossos dados
export interface Message {
  id: number;
  text: string;
  sender: 'me' | 'other';
  status?: 'sent' | 'sending' | 'failed'; // Status opcional para mensagens
}

export interface LogEntry {
  id: number;
  level: 'INFO' | 'CRYPTO' | 'NETWORK' | string;
  message: string;
  timestamp: string;
  color?: string;
}

function App() {
  const [isSidebarMinimized, setIsSidebarMinimized] = useState(true);
  const [activeView, setActiveView] = useState('chat');
  
  // Novos estados que vamos gerenciar
  const [isServerOnline, setIsServerOnline] = useState(false);
  const [messages, setMessages] = useState<Message[]>([]);
  const [logs, setLogs] = useState<LogEntry[]>([]);


  const toggleSidebar = () => setIsSidebarMinimized(!isSidebarMinimized);
  const toggleServer = () => setIsServerOnline(!isServerOnline);

  const addLog = (level: LogEntry['level'], message: string) => {
    const newLog: LogEntry = {
      id: Date.now(),
      level,
      message,
      timestamp: new Date().toLocaleTimeString('pt-BR', {
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
      }),
    };
    setLogs(prevLogs => [...prevLogs, newLog]);
  };

  const deleteAllLogs = () => {
    setLogs([]);
    addLog('INFO', 'Todos os logs foram apagados.');
  }

  useEffect(() => {
    const handleContextMenu = (e: MouseEvent) => {
      e.preventDefault();
    };

    document.addEventListener('contextmenu', handleContextMenu);

    return () => {
      document.removeEventListener('contextmenu', handleContextMenu);
    };
  }, []); // O array vazio [] garante que este efeito rode apenas uma vez.

  // Função para adicionar novas mensagens (simulação)
  const handleSendMessage = (newMessage: string) => {
    const newMsg: Message = {
      id: messages.length + 1,
      text: newMessage,
      sender: 'me',
    };
    setMessages([...messages, newMsg]);
  };

  return (
    <div className="app-container">
      <Sidebar
        isMinimized={isSidebarMinimized}
        toggleSidebar={toggleSidebar}
        setActiveView={setActiveView}
        isServerOnline={isServerOnline}
        activeView={activeView}
      />
      <MainContent
        activeView={activeView}
        // Passando os estados e funções para o conteúdo principal
        isServerOnline={isServerOnline}
        toggleServer={toggleServer}
        messages={messages}
        sendMessage={handleSendMessage}
        logs={logs}
        deleteAllLogs={deleteAllLogs}
      />
    </div>
  );
}

export default App;

