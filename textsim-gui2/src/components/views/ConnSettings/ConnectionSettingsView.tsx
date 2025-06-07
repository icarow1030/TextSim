import React, { useState } from 'react';
import { FaSave, FaSyncAlt } from 'react-icons/fa';
import './ConnectionSettingsView.css';

const ConnectionSettingsView: React.FC = () => {
  // Estados para simular os valores dos inputs
  const [currentPort, setCurrentPort] = useState('8071');
  const [targetPort, setTargetPort] = useState('8070');
  
  return (
    <div className="settings-view">
      <h2>Configurações de Conexão</h2>
      
      {/* Card de Portas */}
      <div className="settings-card">
        <div className="card-header">
          <h3>Portas de Rede</h3>
        </div>
        <div className="card-body">
          <div className="form-group">
            <label htmlFor="current-port">Sua Porta (Servidor)</label>
            <input 
              id="current-port" 
              type="number" 
              value={currentPort}
              onChange={(e) => setCurrentPort(e.target.value)} 
              placeholder="Ex: 8071" 
            />
          </div>
          <div className="form-group">
            <label htmlFor="target-port">Porta de Destino</label>
            <input 
              id="target-port" 
              type="number" 
              value={targetPort}
              onChange={(e) => setTargetPort(e.target.value)}
              placeholder="Ex: 8070" 
            />
          </div>
        </div>
        <div className="card-footer">
          <button className="action-btn save-btn">
            <FaSave />
            <span>Salvar Portas</span>
          </button>
        </div>
      </div>

      {/* Card de Status */}
      <div className="settings-card">
        <div className="card-header">
          <h3>Informações e Status</h3>
        </div>
        <div className="card-body">
          <div className="info-group">
            <span>Sua Porta Atual:</span>
            <strong>{currentPort || 'N/A'}</strong>
          </div>
          <div className="info-group">
            <span>Porta de Destino:</span>
            <strong>{targetPort || 'N/A'}</strong>
          </div>
        </div>
        <div className="card-footer">
          <button className="action-btn check-btn">
            <FaSyncAlt />
            <span>Verificar Status da Conexão</span>
          </button>
        </div>
      </div>

    </div>
  );
};

export default ConnectionSettingsView;