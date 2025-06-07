// src/components/views/ServerView.tsx
import React from 'react';
import './ServerView.css'; // Importando o CSS para estilização
import { FaPlay, FaStop } from 'react-icons/fa'; // Ícones para o botão

interface ServerViewProps {
  isOnline: boolean;
  onToggle: () => void;
}

const ServerView: React.FC<ServerViewProps> = ({ isOnline, onToggle }) => {
  return (
    // O container principal agora é um "card"
    <div className="server-card">
      
      <div className="card-header">
        <h2>Painel do Servidor</h2>
      </div>

      <div className="card-body">
        <div className="status-display">
          <div className={`status-indicator ${isOnline ? 'online' : 'offline'}`}></div>
          <span className={`status-text ${isOnline ? 'online' : 'offline'}`}>
            {isOnline ? 'Online' : 'Offline'}
          </span>
        </div>

        <div className="server-details">
          <div className="detail-item">
            <span>Host:</span>
            <strong>localhost</strong>
          </div>
          <div className="detail-item">
            <span>Porta:</span>
            <strong>{isOnline ? '8071' : 'N/A'}</strong>
          </div>
        </div>
      </div>

      <div className="card-footer">
        <button className={`server-toggle-btn ${isOnline ? 'stop-btn' : 'start-btn'}`} onClick={onToggle}>
          {isOnline ? <FaStop /> : <FaPlay />}
          <span>{isOnline ? 'Parar Servidor' : 'Iniciar Servidor'}</span>
        </button>
      </div>

    </div>
  );
};

export default ServerView;