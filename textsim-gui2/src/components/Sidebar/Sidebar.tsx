// src/components/Sidebar/Sidebar.tsx
import React, { useState, useEffect } from 'react';
// Adicione o getVersion da API do Tauri e o ícone do GitHub
import { getVersion } from '@tauri-apps/api/app';
import { FaBars, FaPowerOff, FaCommentDots, FaCog, FaUserCog, FaSignOutAlt, FaClipboardList, FaBook, FaGithub } from 'react-icons/fa';
import './Sidebar.css';

interface SidebarProps {
  isMinimized: boolean;
  isServerOnline: boolean;
  activeView: string;
  toggleSidebar: () => void;
  setActiveView: (view: string) => void;
}

const Sidebar: React.FC<SidebarProps> = ({ isMinimized, isServerOnline, activeView, toggleSidebar, setActiveView }) => {
  const [appVersion, setAppVersion] = useState('');

  // Hook para buscar a versão do app do tauri.conf.json
  useEffect(() => {
    const fetchVersion = async () => {
      const version = await getVersion();
      setAppVersion(version);
    };
    fetchVersion();
  }, []); // O array vazio garante que rode apenas uma vez

  const navItems = [
    { id: 'server', icon: <FaPowerOff style={{ color: isServerOnline ? 'var(--green)' : 'var(--red)' }} />, text: 'Servidor', view: 'server' },
    { id: 'chat', icon: <FaCommentDots />, text: 'Chat', view: 'chat' },
    { id: 'logs', icon: <FaClipboardList />, text: 'Logs', view: 'logs' },
    { id: 'manual', icon: <FaBook />, text: 'Manual', view: 'manual' },
    { id: 'connection', icon: <FaCog />, text: 'Conexão', view: 'connection' },
    { id: 'user', icon: <FaUserCog />, text: 'Usuário', view: 'user' },
    { id: 'exit', icon: <FaSignOutAlt />, text: 'Sair', view: 'exit' },
  ];

  return (
    <nav className={`sidebar ${isMinimized ? 'minimized' : ''}`}>
      <div className="logo-container">
        <button className="toggle-btn" onClick={toggleSidebar}>
          <FaBars />
        </button>
        <span className="sidebar-app-name">TextSim</span>
      </div>

      <ul className="nav-list">
        {navItems.map(item => (
          <li key={item.id} className={`nav-item ${activeView === item.view ? 'active' : ''}`}>
            <a href="#" onClick={() => setActiveView(item.view)}>
              <div className="nav-icon">{item.icon}</div>
              <span className="nav-text">{item.text}</span>
            </a>
          </li>
        ))}
      </ul>

      {/* NOVO: Rodapé com os créditos e informações */}
      <div className="sidebar-footer">
        <a href="https://github.com/icarow1030" target="_blank" rel="noopener noreferrer" title="Ver perfil no GitHub">
          <div className="nav-icon">
            <FaGithub />
          </div>
          <div className="footer-text-container">
            <span className="footer-title">TextSim v{appVersion}</span>
            <span className="footer-author">by icarow1030</span>
          </div>
        </a>
      </div>
    </nav>
  );
};

export default Sidebar;