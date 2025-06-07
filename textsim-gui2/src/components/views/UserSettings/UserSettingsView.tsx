import React, { useState } from 'react';
import { FaSave, FaKey } from 'react-icons/fa';
// Importa o mesmo CSS para manter a consistência visual
import './UserSettingsView.css'; 

const UserSettingsView: React.FC = () => {
  const [username, setUsername] = useState('YcroSamsara');

  return (
    <div className="settings-view">
      <h2>Configurações de Usuário</h2>

      {/* Card de Perfil */}
      <div className="settings-card">
        <div className="card-header">
          <h3>Perfil</h3>
        </div>
        <div className="card-body">
          <div className="form-group">
            <label htmlFor="username">Nome de Usuário</label>
            <input 
              id="username" 
              type="text" 
              value={username}
              onChange={(e) => setUsername(e.target.value)}
              placeholder="Seu nome de usuário"
            />
          </div>
        </div>
        <div className="card-footer">
          <button className="action-btn save-btn">
            <FaSave />
            <span>Salvar Nome</span>
          </button>
        </div>
      </div>

      {/* Card de Criptografia */}
      <div className="settings-card">
        <div className="card-header">
          <h3>Criptografia</h3>
        </div>
        <div className="card-body">
          <div className="info-group">
            <span>Chave Pública:</span>
            <strong title="d4a3b2c1...">d4a3b2c1... (hash simulado)</strong>
          </div>
          <p style={{color: 'var(--text-dark)', fontSize: '0.9rem', margin: '10px 0 0 0'}}>
            Gere um novo par de chaves RSA para garantir a segurança. A chave pública será compartilhada durante o handshake.
          </p>
        </div>
        <div className="card-footer">
          <button className="action-btn" style={{backgroundColor: 'var(--red)'}}>
            <FaKey />
            <span>Gerar Novas Chaves RSA</span>
          </button>
        </div>
      </div>
    </div>
  );
};

export default UserSettingsView;